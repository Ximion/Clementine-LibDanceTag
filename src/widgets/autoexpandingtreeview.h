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

#ifndef AUTOEXPANDINGTREEVIEW_H
#define AUTOEXPANDINGTREEVIEW_H

#include <QTreeView>

class AutoExpandingTreeView : public QTreeView {
  Q_OBJECT

 public:
  AutoExpandingTreeView(QWidget* parent = 0);

  static const int kRowsToShow;

  void SetAutoOpen(bool v) { auto_open_ = v; }
  void SetExpandOnReset(bool v) { expand_on_reset_ = v; }

 public slots:
  void RecursivelyExpand(const QModelIndex &index);

 protected:
  // QAbstractItemView
  void reset();

 private slots:
  void ItemExpanded(const QModelIndex& index);

 private:
  bool RecursivelyExpand(const QModelIndex& index, int* count);

 private:
  bool auto_open_;
  bool expand_on_reset_;
};

#endif // AUTOEXPANDINGTREEVIEW_H