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

#ifndef MERGEDPROXYMODEL_H
#define MERGEDPROXYMODEL_H

#include <QAbstractProxyModel>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>

using boost::multi_index::multi_index_container;
using boost::multi_index::indexed_by;
using boost::multi_index::hashed_unique;
using boost::multi_index::ordered_unique;
using boost::multi_index::tag;
using boost::multi_index::member;
using boost::multi_index::identity;

std::size_t hash_value(const QModelIndex& index);

class MergedProxyModel : public QAbstractProxyModel {
  Q_OBJECT

 public:
  MergedProxyModel(QObject* parent = 0);
  ~MergedProxyModel();

  // Make another model appear as a child of the given item in the source model.
  void AddSubModel(const QModelIndex& source_parent, const QAbstractItemModel* submodel);

  // Find the item in the source model that is the parent of the model
  // containing proxy_index.  If proxy_index is in the source model, then
  // this just returns mapToSource(proxy_index).
  QModelIndex FindSourceParent(const QModelIndex& proxy_index) const;

  // QAbstractItemModel
  QModelIndex index(int row, int column, const QModelIndex &parent) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const;
  bool hasChildren(const QModelIndex &parent) const;
  QMap<int, QVariant> itemData(const QModelIndex &proxyIndex) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);
  QStringList mimeTypes() const;
  QMimeData* mimeData(const QModelIndexList &indexes) const;

  // QAbstractProxyModel
  // Note that these implementations of map{To,From}Source will not always
  // give you an index in sourceModel(), you might get an index in one of the
  // child models instead.
  QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
  QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
  void setSourceModel(QAbstractItemModel *sourceModel);

 private slots:
  void SourceModelReset();
  void SubModelReset();

  void RowsAboutToBeInserted(const QModelIndex& source_parent, int start, int end);
  void RowsInserted(const QModelIndex& source_parent, int start, int end);
  void RowsAboutToBeRemoved(const QModelIndex& source_parent, int start, int end);
  void RowsRemoved(const QModelIndex& source_parent, int start, int end);

 private:
  QModelIndex GetActualSourceParent(const QModelIndex& source_parent,
                                    const QAbstractItemModel* model) const;
  void DeleteAllMappings();

  struct Mapping {
    Mapping(const QModelIndex& _source_index)
      : source_index(_source_index) {}

    QModelIndex source_index;
  };

  struct tag_by_source {};
  struct tag_by_pointer {};
  typedef multi_index_container<
    Mapping*,
    indexed_by<
      hashed_unique<tag<tag_by_source>,
        member<Mapping, QModelIndex, &Mapping::source_index> >,
      ordered_unique<tag<tag_by_pointer>,
        identity<Mapping*> >
    >
  > MappingContainer;

  MappingContainer mappings_;
  QMap<const QAbstractItemModel*, QModelIndex> merge_points_;
  const QAbstractItemModel* resetting_model_;
};

#endif // MERGEDPROXYMODEL_H