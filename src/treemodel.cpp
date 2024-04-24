#include "treemodel.h"
#include "treeitem.h"

#define COLUMN_NAME 0
#define COLUMN_STATUS 1

TreeModel::TreeModel(TreeFile *tree_file)
    : QAbstractItemModel(), _tree_file(tree_file) {}

QModelIndex TreeModel::index(int row, int, const QModelIndex &parent) const {
  if (!parent.isValid()) {
    auto root = _tree_file->root(row);

    return createIndex(row, 0, root._id);
  }

  auto item = _tree_file->item(parent.internalId(), row);

  return createIndex(row, 0, item._id);
}

QModelIndex TreeModel::parent(const QModelIndex &index) const {
  // The model tester calls this function with an invalid index.
  if (!index.isValid()) {
    return QModelIndex();
  }

  auto item = _tree_file->item(index.internalId());

  // Tests if this is a root, in which case it has no parent.
  if (item._parent == 0) {
    return QModelIndex();
  }

  auto parent = _tree_file->item(item._parent);

  return createIndex(parent._row, 0, parent._id);
}

bool TreeModel::hasChildren(const QModelIndex &index) const {
  // Tests if this if the invisible root, which must have children.
  if (!index.isValid()) {
    return true;
  }

  return _tree_file->childCount(index.internalId()) > 0;
}

int TreeModel::rowCount(const QModelIndex &parent) const {
  if (!parent.isValid()) {
    return _tree_file->rootCount();
  }

  return _tree_file->childCount(parent.internalId());
}

int TreeModel::columnCount(const QModelIndex &) const { return 1; }

QVariant TreeModel::headerData(int section, Qt::Orientation, int role) const {
  QVariant result;
  if (role == Qt::DisplayRole) {
    switch (section) {
    case COLUMN_NAME:
      result = "Name";
      break;
    case COLUMN_STATUS:
      result = "Status";
      break;
    default:
      result = "Unknown";
      break;
    }
  }

  return result;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const {
  switch (role) {
  case Qt::DisplayRole:
  case Qt::EditRole:
    return _tree_file->markdown(index.internalId());

  default:
    return QVariant();
  }
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value,
                        int role) {
  auto markdown = value.toString();

  switch (role) {
  case Qt::EditRole:
    return _tree_file->setMarkdown(index.internalId(), markdown);
  default:
    return false;
  }
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const {
  if (!index.isValid()) {
    return Qt::NoItemFlags;
  }

  return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

bool TreeModel::insertRows(int row, int count, const QModelIndex &parent) {
  beginInsertRows(parent, row, row + count - 1);

  auto ok = true;
  for (auto i = 0; i < count; ++i) {
    ok &= _tree_file->insert(parent.internalId(), row + i);
  }

  endInsertRows();

  return ok;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex &parent) {
  beginRemoveRows(parent, row, row + count - 1);

  auto ok = true;
  for (auto i = 0; i < count; ++i) {
    ok &= _tree_file->remove(parent.internalId(), row + i);
  }

  endRemoveRows();

  return ok;
}

bool TreeModel::moveRows(const QModelIndex &sourceParent, int sourceRow,
                         int count, const QModelIndex &destinationParent,
                         int destinationRow) {
  if (destinationRow < 0 ||
      destinationRow >= _tree_file->childCount(sourceParent.internalId())) {
    return false;
  }

  // IMPORTANT: when moving rows up, they will be placed before the
  // `destinationRow`, but when moving rows down space must be allocated before
  // `destinationRow` so they will be placed before correctly.
  // https://doc.qt.io/qt-6/qabstractitemmodel.html#beginMoveRows
  auto offset = destinationRow < sourceRow ? 0 : count;
  beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1,
                destinationParent, destinationRow + offset);
  auto ok = true;
  for (auto i = 0; i < count; ++i) {
    // TODO: only support moving items within the same parent for now.
    ok &= _tree_file->moveItem(sourceRow + i, destinationRow + i,
                               sourceParent.internalId());
  }

  endMoveRows();

  return ok;
}

QModelIndex TreeModel::appendChild(const QModelIndex &parent) {
  int row = _tree_file->childCount(parent.internalId());

  if (insertRow(row, parent)) {
    auto item = _tree_file->last();

    return createIndex(item._row, 0, item._id);
  }

  return QModelIndex();
}

QModelIndex TreeModel::appendSibling(const QModelIndex &index) {
  if (insertRow(index.row() + 1, index.parent())) {
    auto item = _tree_file->last();

    return createIndex(item._row, 0, item._id);
  }

  return QModelIndex();
}

QModelIndex TreeModel::prependSibling(const QModelIndex &index) {
  if (insertRow(index.row(), index.parent())) {
    auto item = _tree_file->last();

    return createIndex(item._row, 0, item._id);
  }

  return QModelIndex();
}

void TreeModel::removeItem(const QModelIndex &index) {
  removeRow(index.row(), index.parent());
}

void TreeModel::moveItem(const QModelIndex &index, int step) {
  moveRow(index.parent(), index.row(), index.parent(), index.row() + step);
}
