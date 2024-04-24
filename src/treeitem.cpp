#include "treeitem.h"

TreeItem::TreeItem(int id, int parent, int row, QString &markdown)
    : _id(id), _parent(parent), _row(row), _markdown(markdown) {}
