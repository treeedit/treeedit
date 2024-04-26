#include "treedelegate.h"

#include <QPainter>
#include <QPlainTextEdit>

TreeDelegate::TreeDelegate() : _document(std::make_unique<QTextDocument>()) {}

void TreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const {
  auto markdown = index.data(Qt::DisplayRole).toString();
  _document->setMarkdown(markdown, QTextDocument::MarkdownDialectCommonMark);
  _document->setDefaultFont(option.font);
  _document->setTextWidth(option.rect.width());

  // TODO: maybe use `setDefaultStyleSheet` instead, to support bold and italic.
  auto id = QFontDatabase::addApplicationFont(":/fonts/inter-regular-v42.ttf");
  auto family = QFontDatabase::applicationFontFamilies(id).at(0);
  QFont inter(family);
  _document->setDefaultFont(inter);

  auto background = option.state & QStyle::State_Selected
      ? option.palette.highlight()
                        : option.palette.base();

  // https://www.w3.org/TR/SVG11/types.html#ColorKeywords
  if (markdown.startsWith("TODO:")) {
      background.setColor(QColor("yellow"));
  } else if (markdown.startsWith("DONE:")) {
      background.setColor(QColor("green"));
  } else if (markdown.startsWith("BUG:")) {
      background.setColor(QColor("orange"));
  } else if (markdown.startsWith("ETA:")) {
      background.setColor(QColor("purple"));
  } else if (markdown.startsWith("P0:")) {
      background.setColor(QColor("red"));
  } else if (markdown.startsWith("P1:")) {
      background.setColor(QColor("orange"));
  } else if (markdown.startsWith("P2:")) {
      background.setColor(QColor("yellow"));
  } else if (markdown.startsWith("P3:")) {
      background.setColor(QColor("purple"));
  }

  painter->fillRect(option.rect, background);

  painter->save();
  painter->translate(option.rect.topLeft());

  _document->drawContents(painter);

  painter->restore();
}

QSize TreeDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const {
  auto markdown = index.data(Qt::DisplayRole).toString();
  _document->setMarkdown(markdown, QTextDocument::MarkdownDialectCommonMark);
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
