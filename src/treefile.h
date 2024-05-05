#ifndef TREEFILE_H
#define TREEFILE_H

#include <QSqlDatabase>
#include <QString>
#include <QVector>

#include "treeitem.h"

class TreeFile {
public:
  TreeFile(const QString &path);
  ~TreeFile();

  bool create();
  bool reset();
  bool integrityCheck() const;

  int rootCount() const;
  int childCount(int id) const;

  int id(int parent, int row) const;
  TreeItem root(int row) const;
  TreeItem item(int id) const;
  TreeItem item(int parent, int row) const;
  TreeItem last() const;

  QString markdown(int id) const;
  bool setMarkdown(int id, QString &value);

  bool insert(int parent, int row);
  bool remove(int parent, int row);
  bool moveItem(int sourceParent, int sourceRow, int destinationParent, int destinationRow);

private:
  QSqlDatabase _db;
};

#endif // TREEFILE_H
