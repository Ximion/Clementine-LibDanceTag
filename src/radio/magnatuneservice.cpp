/* This file is part of Clementine.

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "magnatuneplaylistitem.h"
#include "magnatuneservice.h"
#include "radiomodel.h"
#include "core/mergedproxymodel.h"
#include "core/networkaccessmanager.h"
#include "core/song.h"
#include "library/librarymodel.h"
#include "library/librarybackend.h"
#include "library/libraryfilterwidget.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QtIOCompressor>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QDesktopServices>
#include <QCoreApplication>

#include <QtDebug>

using boost::shared_ptr;

const char* MagnatuneService::kServiceName = "Magnatune";
const char* MagnatuneService::kSettingsGroup = "Magnatune";
const char* MagnatuneService::kDatabaseUrl =
    "http://magnatune.com/info/song_info_xml.gz";
const char* MagnatuneService::kSongsTable = "magnatune_songs";
const char* MagnatuneService::kHomepage = "http://magnatune.com";

MagnatuneService::MagnatuneService(RadioModel* parent)
  : RadioService(kServiceName, parent),
    root_(NULL),
    context_menu_(new QMenu),
    library_backend_(new LibraryBackend(parent->db(), kSongsTable,
                                        QString::null, QString::null, this)),
    library_model_(new LibraryModel(library_backend_, this)),
    library_sort_model_(new QSortFilterProxyModel(this)),
    total_song_count_(0),
    network_(parent->network()->network())
{
  connect(library_backend_, SIGNAL(TotalSongCountUpdated(int)),
          SLOT(UpdateTotalSongCount(int)));

  library_sort_model_->setSourceModel(library_model_);
  library_sort_model_->setSortRole(LibraryModel::Role_SortText);
  library_sort_model_->setDynamicSortFilter(true);
  library_sort_model_->sort(0);

  add_to_playlist_ = context_menu_->addAction(
      QIcon(":media-playback-start.png"), tr("Add to playlist"), this, SLOT(AddToPlaylist()));
  context_menu_->addSeparator();
  context_menu_->addAction(QIcon(":web.png"), tr("Open magnatune.com in browser"), this, SLOT(Homepage()));
  context_menu_->addAction(QIcon(":refresh.png"), tr("Refresh catalogue"), this, SLOT(ReloadDatabase()));

  library_model_->Init();
}

MagnatuneService::~MagnatuneService() {
  delete context_menu_;
}

RadioItem* MagnatuneService::CreateRootItem(RadioItem *parent) {
  root_ = new RadioItem(this, RadioItem::Type_Service, kServiceName, parent);
  root_->icon = QIcon(":magnatune.png");

  model()->merged_model()->AddSubModel(
      model()->index(root_->row, 0, model()->ItemToIndex(parent)),
      library_sort_model_);

  return root_;
}

void MagnatuneService::LazyPopulate(RadioItem *item) {
  switch (item->type) {
    case RadioItem::Type_Service:
      if (total_song_count_ == 0)
        ReloadDatabase();
      break;

    default:
      break;
  }

  item->lazy_loaded = true;
}

void MagnatuneService::StartLoading(const QUrl& url) {
  emit StreamReady(url, url);
}

void MagnatuneService::ReloadDatabase() {
  QNetworkRequest request = QNetworkRequest(QUrl(kDatabaseUrl));
  request.setRawHeader("User-Agent", QString("%1 %2").arg(
      QCoreApplication::applicationName(), QCoreApplication::applicationVersion()).toUtf8());

  QNetworkReply* reply = network_->get(request);
  connect(reply, SIGNAL(finished()), SLOT(ReloadDatabaseFinished()));
  
  emit TaskStarted(MultiLoadingIndicator::LoadingMagnatune);
}

void MagnatuneService::ReloadDatabaseFinished() {
  QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

  emit TaskFinished(MultiLoadingIndicator::LoadingMagnatune);
  root_->lazy_loaded = true;

  if (reply->error() != QNetworkReply::NoError) {
    // TODO: Error handling
    qDebug() << reply->errorString();
    return;
  }

  root_->ClearNotify();

  // The XML file is compressed
  QtIOCompressor gzip(reply);
  gzip.setStreamFormat(QtIOCompressor::GzipFormat);
  if (!gzip.open(QIODevice::ReadOnly)) {
    qWarning() << "Error opening gzip stream";
    return;
  }

  // Remove all existing songs in the database
  // FindSongsByDirectory isn't the nicest way to do it, but it's easy
  SongList songs = library_backend_->FindSongsInDirectory(0);
  library_backend_->DeleteSongs(songs);
  songs.clear();

  // Parse the XML we got from Magnatune
  QXmlStreamReader reader(&gzip);
  while (!reader.atEnd()) {
    reader.readNext();

    if (reader.tokenType() == QXmlStreamReader::StartElement &&
        reader.name() == "Track") {
      songs << ReadTrack(reader);
    }
  }

  // Add the songs to the database
  library_backend_->AddOrUpdateSongs(songs);
}

Song MagnatuneService::ReadTrack(QXmlStreamReader& reader) {
  Song song;

  while (!reader.atEnd()) {
    reader.readNext();

    if (reader.tokenType() == QXmlStreamReader::EndElement)
      break;

    if (reader.tokenType() == QXmlStreamReader::StartElement) {
      QString value = ReadElementText(reader);

      if (reader.name() == "artist")          song.set_artist(value);
      if (reader.name() == "albumname")       song.set_album(value);
      if (reader.name() == "trackname")       song.set_title(value);
      if (reader.name() == "tracknum")        song.set_track(value.toInt());
      if (reader.name() == "year")            song.set_year(value.toInt());
      if (reader.name() == "magnatunegenres") song.set_genre(value.section(',', 0, 0));
      if (reader.name() == "seconds")         song.set_length(value.toInt());
      if (reader.name() == "url")             song.set_filename(value);
      if (reader.name() == "cover_small")     song.set_art_automatic(value);
    }
  }

  song.set_valid(true);
  song.set_filetype(Song::Type_Stream);

  // We need to set these to satisfy the database constraints
  song.set_directory_id(0);
  song.set_mtime(0);
  song.set_ctime(0);
  song.set_filesize(0);

  return song;
}

// TODO: Replace with readElementText(SkipChildElements) in Qt 4.6
QString MagnatuneService::ReadElementText(QXmlStreamReader& reader) {
  int level = 1;
  QString ret;
  while (!reader.atEnd()) {
    switch (reader.readNext()) {
      case QXmlStreamReader::StartElement: level++; break;
      case QXmlStreamReader::EndElement:   level--; break;
      case QXmlStreamReader::Characters:
        ret += reader.text().toString().trimmed();
        break;
      default: break;
    }

    if (level == 0)
      break;
  }
  return ret;
}

void MagnatuneService::ShowContextMenu(RadioItem*, const QModelIndex& index,
                                       const QPoint& global_pos) {
  if (index.model() == library_sort_model_)
    context_item_ = index;
  else
    context_item_ = QModelIndex();

  add_to_playlist_->setEnabled(context_item_.isValid());
  context_menu_->popup(global_pos);
}

void MagnatuneService::AddToPlaylist() {
  SongList songs(library_model_->GetChildSongs(
      library_sort_model_->mapToSource(context_item_)));

  PlaylistItemList items;

  foreach (const Song& song, songs) {
    items << shared_ptr<PlaylistItem>(new MagnatunePlaylistItem(song));
  }

  emit AddItemsToPlaylist(items);
}

void MagnatuneService::Homepage() {
  QDesktopServices::openUrl(QUrl(kHomepage));
}

bool MagnatuneService::SetupLibraryFilter(LibraryFilterWidget* w) const {
  w->SetSettingsGroup(kSettingsGroup);
  w->SetLibraryModel(library_model_);
  w->SetFilterHint(tr("Search Magnatune"));
  w->SetAgeFilterEnabled(false);
  w->SetConfigDialogEnabled(false);

  return true;
}