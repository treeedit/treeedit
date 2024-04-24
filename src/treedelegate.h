#ifndef TREEDELEGATE_H
#define TREEDELEGATE_H

#include <QStyledItemDelegate>
#include <QTextDocument>

class TreeDelegate : public QStyledItemDelegate {
public:
  TreeDelegate();

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const override;
  QSize sizeHint(const QStyleOptionViewItem &option,
                 const QModelIndex &index) const override;

  // QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
  //                       const QModelIndex &index) const override;
  // void setEditorData(QWidget *editor, const QModelIndex &index) const
  // override; void setModelData(QWidget *editor, QAbstractItemModel *model,
  //                   const QModelIndex &index) const override;

private:
  std::unique_ptr<QTextDocument> _document;
};

#endif // TREEDELEGATE_H
