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

#include "osd.h"
#include "osdpretty.h"

#include <QCoreApplication>
#include <QtDebug>
#include <QSettings>

const char* OSD::kSettingsGroup = "OSD";

OSD::OSD(QSystemTrayIcon* tray_icon, NetworkAccessManager* network, QObject* parent)
  : QObject(parent),
    tray_icon_(tray_icon),
    timeout_msec_(5000),
    behaviour_(Native),
    show_on_volume_change_(false),
    show_art_(true),
    force_show_next_(false),
    ignore_next_stopped_(false),
    pretty_popup_(new OSDPretty(OSDPretty::Mode_Popup)),
    network_(network),
    cover_loader_(new BackgroundThreadImplementation<AlbumCoverLoader, AlbumCoverLoader>(this))
{
  cover_loader_->Start();

  connect(cover_loader_, SIGNAL(Initialised()), SLOT(CoverLoaderInitialised()));

  ReloadSettings();
  Init();
}

OSD::~OSD() {
  delete pretty_popup_;
}

void OSD::CoverLoaderInitialised() {
  cover_loader_->Worker()->SetNetwork(network_);
  connect(cover_loader_->Worker().get(), SIGNAL(ImageLoaded(quint64,QImage)),
          SLOT(AlbumArtLoaded(quint64,QImage)));
}

void OSD::ReloadSettings() {
  QSettings s;
  s.beginGroup(kSettingsGroup);
  behaviour_ = OSD::Behaviour(s.value("Behaviour", Native).toInt());
  timeout_msec_ = s.value("Timeout", 5000).toInt();
  show_on_volume_change_ = s.value("ShowOnVolumeChange", false).toBool();
  show_art_ = s.value("ShowArt", true).toBool();

  if (!SupportsNativeNotifications() && behaviour_ == Native)
    behaviour_ = Pretty;
  if (!SupportsTrayPopups() && behaviour_ == TrayPopup)
    behaviour_ = Disabled;

  pretty_popup_->set_popup_duration(timeout_msec_);
  pretty_popup_->ReloadSettings();
}

void OSD::SongChanged(const Song &song) {
  QString summary(song.PrettyTitle());
  if (!song.artist().isEmpty())
    summary = QString("%1 - %2").arg(song.artist(), summary);

  QStringList message_parts;
  if (!song.album().isEmpty())
    message_parts << song.album();
  if (song.disc() > 0)
    message_parts << tr("disc %1").arg(song.disc());
  if (song.track() > 0)
    message_parts << tr("track %1").arg(song.track());

  WaitingForAlbumArt waiting;
  waiting.icon = "notification-audio-play";
  waiting.summary = summary;
  waiting.message = message_parts.join(", ");

  if (show_art_) {
    quint64 id = cover_loader_->Worker()->LoadImageAsync(
        song.art_automatic(), song.art_manual());
    waiting_for_album_art_.insert(id, waiting);
  } else {
    AlbumArtLoaded(waiting, QImage());
  }
}

void OSD::AlbumArtLoaded(quint64 id, const QImage& image) {
  WaitingForAlbumArt info = waiting_for_album_art_.take(id);
  AlbumArtLoaded(info, image);
}

void OSD::AlbumArtLoaded(const WaitingForAlbumArt info, const QImage& image) {
  ShowMessage(info.summary, info.message, info.icon, image);
}

void OSD::Paused() {
  ShowMessage(QCoreApplication::applicationName(), tr("Paused"));
}

void OSD::Stopped() {
  if (ignore_next_stopped_) {
    ignore_next_stopped_ = false;
    return;
  }

  ShowMessage(QCoreApplication::applicationName(), tr("Stopped"));
}

void OSD::PlaylistFinished() {
  // We get a PlaylistFinished followed by a Stopped from the player
  ignore_next_stopped_ = true;

  ShowMessage(QCoreApplication::applicationName(), tr("Playlist finished"));
}

void OSD::VolumeChanged(int value) {
  if (!show_on_volume_change_)
    return;

  ShowMessage(QCoreApplication::applicationName(), tr("Volume %1%").arg(value));
}

void OSD::ShowMessage(const QString& summary,
                      const QString& message,
                      const QString& icon,
                      const QImage& image) {
  switch (behaviour_) {
    case Native:
      if (image.isNull()) {
        ShowMessageNative(summary, message, icon, QImage());
      } else {
        ShowMessageNative(summary, message, QString(), image);
      }
      break;

#ifndef Q_OS_DARWIN
    case TrayPopup:
      tray_icon_->showMessage(summary, message, QSystemTrayIcon::NoIcon, timeout_msec_);
      break;
#endif

    case Disabled:
      if (!force_show_next_)
        break;
      force_show_next_ = false;
      // fallthrough
    case Pretty:
      pretty_popup_->SetMessage(summary, message, image);
      pretty_popup_->show();
      break;

    default:
      break;
  }
}

#ifndef Q_WS_X11
void OSD::CallFinished(QDBusPendingCallWatcher*) {}
#endif