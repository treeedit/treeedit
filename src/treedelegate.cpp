#include "treedelegate.h"

#include <QPainter>
#include <QPlainTextEdit>

TreeDelegate::TreeDelegate() : _document(std::make_unique<QTextDocument>()) {}

void TreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const {
  QVariant markdown = index.data(Qt::DisplayRole);
  _document->setMarkdown(markdown.toString(), QTextDocument::MarkdownDialectCommonMark);
  _document->setDefaultFont(option.font);
  _document->setTextWidth(option.rect.width());

  // TODO: maybe use `setDefaultStyleSheet` instead, to support bold and italic.
  auto id = QFontDatabase::addApplicationFont(":/fonts/inter-regular-v42.ttf");
  auto family = QFontDatabase::applicationFontFamilies(id).at(0);
  QFont inter(family);
  _document->setDefaultFont(inter);

  if (option.state & QStyle::State_Selected) {
    painter->fillRect(option.rect, option.palette.highlight());
  }

  painter->save();
  painter->translate(option.rect.topLeft());

  _document->drawContents(painter);

  painter->restore();
}

QSize TreeDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const {
  QVariant markdown = index.data(Qt::DisplayRole);
  _document->setMarkdown(markdown.toString(), QTextDocument::MarkdownDialectCommonMark);
  _document->setDefaultFont(option.font);
  _document->setTextWidth(option.rect.width());

  return QSize(_document->idealWidth(), _document->size().height());
}

// QWidget *TreeDelegate::createEditor(QWidget *parent,
//                                     const QStyleOptionViewItem &option,
//                                     const QModelIndex &index) const {
//   auto *editor = new QPlainTextEdit(parent);

//   return editor;
// }

// void TreeDelegate::setEditorData(QWidget *editor,
//                                  const QModelIndex &index) const {
//   auto *plaintext_editor = qobject_cast<QPlainTextEdit *>(editor);
//   plaintext_editor->setPlainText(index.data(Qt::EditRole).toString());
// }

// void TreeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
//                                 const QModelIndex &index) const {
//   auto *plaintext_editor = qobject_cast<QPlainTextEdit *>(editor);
//   model->setData(index, plaintext_editor->toPlainText());
// }
