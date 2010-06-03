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

#include "playlist.h"
#include "playlistbackend.h"
#include "playlistmanager.h"
#include "playlistparsers/playlistparser.h"

#include <QFileInfo>

PlaylistManager::PlaylistManager(QObject *parent)
  : QObject(parent),
    playlist_backend_(NULL),
    library_backend_(NULL),
    sequence_(NULL),
    parser_(new PlaylistParser(this)),
    current_(-1),
    active_(-1)
{
}

PlaylistManager::~PlaylistManager() {
  foreach (const Data& data, playlists_.values()) {
    delete data.p;
  }
}

void PlaylistManager::Init(LibraryBackend* library_backend,
                           PlaylistBackend* playlist_backend,
                           PlaylistSequence* sequence) {
  library_backend_ = library_backend;
  playlist_backend_ = playlist_backend;
  sequence_ = sequence;

  foreach (const PlaylistBackend::Playlist& p, playlist_backend->GetAllPlaylists()) {
    AddPlaylist(p.id, p.name);
  }

  // If no playlist exists then make a new one
  if (playlists_.isEmpty())
    New(tr("Playlist"));
}

Playlist* PlaylistManager::AddPlaylist(int id, const QString& name) {
  Playlist* ret = new Playlist(playlist_backend_, id);
  ret->set_sequence(sequence_);

  connect(ret, SIGNAL(CurrentSongChanged(Song)), SIGNAL(CurrentSongChanged(Song)));
  connect(ret, SIGNAL(PlaylistChanged()), SIGNAL(PlaylistChanged()));
  connect(ret, SIGNAL(EditingFinished(QModelIndex)), SIGNAL(EditingFinished(QModelIndex)));

  playlists_[id] = Data(ret, name);

  emit PlaylistAdded(id, name);

  if (current_ == -1) {
    SetCurrentPlaylist(id);
  }
  if (active_ == -1) {
    SetActivePlaylist(id);
  }

  return ret;
}

void PlaylistManager::New(const QString& name, const SongList& songs) {
  int id = playlist_backend_->CreatePlaylist(name);

  if (id == -1)
    qFatal("Couldn't create playlist");

  Playlist* playlist = AddPlaylist(id, name);
  playlist->InsertSongs(songs);

  SetCurrentPlaylist(id);
}

void PlaylistManager::Load(const QString& filename) {
  SongList songs = parser_->Load(filename);
  QFileInfo info(filename);

  if (songs.isEmpty()) {
    emit Error(tr("The playlist '%1' was empty or could not be loaded.").arg(
        info.completeBaseName()));
    return;
  }

  New(info.baseName(), songs);
}

void PlaylistManager::Save(int id, const QString& filename) {
  Q_ASSERT(playlists_.contains(id));

  parser_->Save(playlist(id)->GetAllSongs(), filename);
}

void PlaylistManager::Rename(int id, const QString& new_name) {
  Q_ASSERT(playlists_.contains(id));

  playlist_backend_->RenamePlaylist(id, new_name);
  playlists_[id].name = new_name;

  emit PlaylistRenamed(id, new_name);
}

void PlaylistManager::Remove(int id) {
  Q_ASSERT(playlists_.contains(id));

  // Won't allow removing the last playlist
  if (playlists_.count() <= 1)
    return;

  playlist_backend_->RemovePlaylist(id);

  int next_id = playlists_.constBegin()->p->id();

  if (id == active_)
    SetActivePlaylist(next_id);
  if (id == current_)
    SetCurrentPlaylist(next_id);

  Data data = playlists_.take(id);
  delete data.p;

  emit PlaylistRemoved(id);
}

void PlaylistManager::SetCurrentPlaylist(int id) {
  Q_ASSERT(playlists_.contains(id));
  current_ = id;
  emit CurrentChanged(current());
}

void PlaylistManager::SetActivePlaylist(int id) {
  Q_ASSERT(playlists_.contains(id));

  // Kinda a hack: unset the current item from the old active playlist before
  // setting the new one
  if (active_ != -1)
    active()->set_current_index(-1);

  active_ = id;
  emit ActiveChanged(active());
}

void PlaylistManager::ClearCurrent() {
  current()->Clear();
}

void PlaylistManager::ShuffleCurrent() {
  current()->Shuffle();
}

void PlaylistManager::SetActivePlaying() {
  active()->Playing();
}

void PlaylistManager::SetActivePaused() {
  active()->Paused();
}

void PlaylistManager::SetActiveStopped() {
  active()->Stopped();
}

void PlaylistManager::SetActiveStreamMetadata(const QUrl &url, const Song &song) {
  active()->SetStreamMetadata(url, song);
}

void PlaylistManager::ChangePlaylistOrder(const QList<int>& ids) {
  playlist_backend_->SetPlaylistOrder(ids);
}