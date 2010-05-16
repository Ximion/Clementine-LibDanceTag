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

#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <QWidget>
#include <QUndoCommand>
#include <QUrl>
#include <QModelIndex>

class Ui_FileView;

class QFileSystemModel;
class QUndoStack;

class FileView : public QWidget {
  Q_OBJECT

 public:
  FileView(QWidget* parent = 0);
  ~FileView();

  void SetPath(const QString& path);

 signals:
  void PathChanged(const QString& path);

  void Load(const QList<QUrl>& urls);
  void AddToPlaylist(const QList<QUrl>& urls);
  void DoubleClicked(const QList<QUrl>& urls);
  void CopyToLibrary(const QList<QUrl>& urls);
  void MoveToLibrary(const QList<QUrl>& urls);

 private slots:
  void FileUp();
  void FileHome();
  void ChangeFilePath(const QString& new_path);
  void ItemActivated(const QModelIndex& index);
  void ItemDoubleClick(const QModelIndex& index);

 private:
  void ChangeFilePathWithoutUndo(const QString& new_path);

 private:
  class UndoCommand : public QUndoCommand {
   public:
    UndoCommand(FileView* view, const QString& new_path);

    QString undo_path() const { return old_state_.path; }

    void undo();
    void redo();

   private:
    struct State {
      State() : scroll_pos(-1) {}

      QString path;
      QModelIndex index;
      int scroll_pos;
    };

    FileView* view_;
    State old_state_;
    State new_state_;
  };

  Ui_FileView* ui_;

  QFileSystemModel* model_;
  QUndoStack* undo_stack_;
};

#endif // FILEVIEW_H