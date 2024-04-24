#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QObject>

#include "treefile.h"

class TreeModel : public QAbstractItemModel {
public:
  TreeModel(TreeFile *treeFile);

  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;

  bool hasChildren(const QModelIndex &) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

  bool insertRows(int row, int count,
                  const QModelIndex &parent = QModelIndex()) override;
  bool removeRows(int row, int count,
                  const QModelIndex &parent = QModelIndex()) override;
  bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
                const QModelIndex &destinationParent,
                int destinationRow) override;

  // custom
  QModelIndex appendChild(const QModelIndex &parent = QModelIndex());
  QModelIndex appendSibling(const QModelIndex &index);
  QModelIndex prependSibling(const QModelIndex &index);
  void removeItem(const QModelIndex &index);
  void moveItem(const QModelIndex &index, int step);

private:
  TreeFile *_tree_file;
};

#endif // TREEMODEL_H
