#ifndef TREEITEM_H
#define TREEITEM_H

#include <QString>

class TreeItem {
public:
  TreeItem(int id, int parent, int row, QString &markdown);

  int _id;
  int _parent;
  int _row;
  QString _markdown;
};

#endif // TREEITEM_H
