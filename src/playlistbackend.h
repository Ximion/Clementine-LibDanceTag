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

#ifndef PLAYLISTBACKEND_H
#define PLAYLISTBACKEND_H

#include <QObject>
#include <QList>

#include "playlistitem.h"

class Database;

class PlaylistBackend : public QObject {
  Q_OBJECT

 public:
  PlaylistBackend(Database* db, QObject* parent = 0);

  struct Playlist {
    int id;
    QString name;
  };
  typedef QList<Playlist> PlaylistList;

  PlaylistList GetAllPlaylists();
  PlaylistItemList GetPlaylistItems(int playlist);
  void SavePlaylistAsync(int playlist, const PlaylistItemList& items);

 public slots:
  void SavePlaylist(int playlist, const PlaylistItemList& items);

 private:
  Database* db_;
};

#endif // PLAYLISTBACKEND_H