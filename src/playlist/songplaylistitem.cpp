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

#include "songplaylistitem.h"

#include <QtDebug>
#include <QFile>
#include <QSettings>

SongPlaylistItem::SongPlaylistItem(const QString& type)
  : PlaylistItem(type)
{
}

SongPlaylistItem::SongPlaylistItem(const Song& song)
  : PlaylistItem(song.filetype() == Song::Type_Stream ? "Stream" : "File"),
    song_(song)
{
}

bool SongPlaylistItem::InitFromQuery(const QSqlQuery &query) {
  // The song tables gets joined first, plus one each for the song ROWIDs
  const int row = (Song::kColumns.count() + 1) * 2;

  QString filename(query.value(row + 1).toString());

  if (type() == "Stream") {
    QString title(query.value(row + 2).toString());
    QString artist(query.value(row + 3).toString());
    QString album(query.value(row + 4).toString());
    int length(query.value(row + 5).toInt());
    if (title.isEmpty())  title = "Unknown";
    if (artist.isEmpty()) artist = "Unknown";
    if (album.isEmpty())  album = "Unknown";
    if (length == 0)      length = -1;

    song_.set_filename(filename);
    song_.set_filetype(Song::Type_Stream);

    song_.Init(title, artist, album, length);
  } else {
    song_.InitFromFile(filename, -1);

    if (!song_.is_valid())
      return false;
  }
  return true;
}

QVariant SongPlaylistItem::DatabaseValue(DatabaseColumn column) const {
  switch (column) {
    case Column_Url:    return song_.filename();
    case Column_Title:  return song_.title();
    case Column_Artist: return song_.artist();
    case Column_Album:  return song_.album();
    case Column_Length: return song_.length();
    default:            return PlaylistItem::DatabaseValue(column);
  }
}

QUrl SongPlaylistItem::Url() const {
  if (QFile::exists(song_.filename())) {
    return QUrl::fromLocalFile(song_.filename());
  } else {
    return song_.filename();
  }
}

void SongPlaylistItem::Reload() {
  song_.InitFromFile(song_.filename(), song_.directory_id());
}