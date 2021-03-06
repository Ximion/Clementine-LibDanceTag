/* This file is part of Clementine.
   Copyright 2010, David Sansome <me@davidsansome.com>

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

#ifndef PLAYLISTTABBAR_H
#define PLAYLISTTABBAR_H

#include <QBasicTimer>
#include <QIcon>
#include <QTabBar>

class PlaylistManager;
class RenameTabLineEdit;

class QMenu;

class PlaylistTabBar : public QTabBar {
  Q_OBJECT

public:
  PlaylistTabBar(QWidget *parent = 0);

  static const int kDragHoverTimeout = 500;

  void SetActions(QAction* new_playlist, QAction* load_playlist);
  void SetManager(PlaylistManager* manager);

  // We use IDs to refer to tabs so the tabs can be moved around (and their
  // indexes change).
  int index_of(int id) const;
  int current_id() const;
  int id_of(int index) const;

  // Utility functions that use IDs rather than indexes
  void set_current_id(int id);
  void set_icon_by_id(int id, const QIcon& icon);
  void set_text_by_id(int id, const QString& text);

  void RemoveTab(int id);
  void InsertTab(int id, int index, const QString& text,
                 const QIcon& icon = QIcon());

signals:
  void CurrentIdChanged(int id);
  void Rename(int id, const QString& name);
  void Remove(int id);
  void Save(int id);
  void PlaylistOrderChanged(const QList<int>& ids);

protected:
  void contextMenuEvent(QContextMenuEvent* e);
  void mouseReleaseEvent(QMouseEvent* e);
  void mouseDoubleClickEvent(QMouseEvent* e);
  void dragEnterEvent(QDragEnterEvent* e);
  void dragMoveEvent(QDragMoveEvent* e);
  void dragLeaveEvent(QDragLeaveEvent* e);
  void dropEvent(QDropEvent* e);
  void timerEvent(QTimerEvent* e);
  bool event(QEvent* e);

private slots:
  void CurrentIndexChanged(int index);
  void Rename();
  void RenameInline();
  void HideEditor();
  void Remove();
  void RemoveFromTabIndex(int index);
  void TabMoved();
  void Save();

private:
  PlaylistManager* manager_;

  QMenu* menu_;
  int menu_index_;
  QAction* new_;
  QAction* rename_;
  QAction* remove_;
  QAction* save_;

  QBasicTimer drag_hover_timer_;
  int drag_hover_tab_;

  bool suppress_current_changed_;

  // Editor for inline renaming
  RenameTabLineEdit* rename_editor_;

  // We want to ask for confirmation only in certain cases
  bool removing_with_confirm_;
};

#endif // PLAYLISTTABBAR_H
