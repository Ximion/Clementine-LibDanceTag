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

#ifndef PLAYLISTCONTAINER_H
#define PLAYLISTCONTAINER_H

#include <QWidget>
#include <QSettings>

class Ui_PlaylistContainer;

class Playlist;
class PlaylistManager;
class PlaylistView;

class QTimeLine;
class QLabel;

class PlaylistContainer : public QWidget {
  Q_OBJECT

public:
  PlaylistContainer(QWidget *parent = 0);
  ~PlaylistContainer();

  static const char* kSettingsGroup;

  void SetActions(QAction* new_playlist, QAction* save_playlist,
                  QAction* load_playlist);
  void SetManager(PlaylistManager* manager);

  PlaylistView* view() const;

signals:
  void TabChanged(int id);
  void Rename(int id, const QString& new_name);
  void Remove(int id);

  void UndoRedoActionsChanged(QAction* undo, QAction* redo);

protected:
  // QWidget
  void resizeEvent(QResizeEvent *);

private slots:
  void ClearFilter();
  void NewPlaylist();
  void LoadPlaylist();
  void SavePlaylist() { SavePlaylist(-1); }
  void SavePlaylist(int id);

  void SetViewModel(Playlist* playlist);
  void PlaylistAdded(int id, const QString& name);
  void PlaylistRemoved(int id);
  void PlaylistRenamed(int id, const QString& new_name);

  void ActivePlaying();
  void ActivePaused();
  void ActiveStopped();

  void Save();

  void SetTabBarVisible(bool visible);
  void SetTabBarHeight(int height);

  void UpdateFilter();

private:
  void UpdateActiveIcon(const QIcon& icon);
  void RepositionNoMatchesLabel(bool force = false);

private:
  Ui_PlaylistContainer* ui_;

  PlaylistManager* manager_;
  QAction* undo_;
  QAction* redo_;

  QSettings settings_;
  bool starting_up_;

  bool tab_bar_visible_;
  QTimeLine* tab_bar_animation_;

  QLabel* no_matches_label_;
};

#endif // PLAYLISTCONTAINER_H