#include "treedelegate.h"

#include <QFont>
#include <QLineEdit>
#include <QPainter>

TreeDelegate::TreeDelegate() : _document(std::make_unique<QTextDocument>()) {}

void TreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const {
    auto opt = option;
    initStyleOption(&opt, index);

    auto markdown = index.data(Qt::DisplayRole).toString();
    _document->setMarkdown(markdown, QTextDocument::MarkdownDialectCommonMark);
    _document->setDefaultFont(opt.font);
    _document->setTextWidth(option.rect.width());

    painter->fillRect(opt.rect, opt.backgroundBrush);

    painter->save();
    painter->translate(opt.rect.topLeft());

    _document->drawContents(painter);

    painter->restore();
}

QSize TreeDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const {
    auto opt = option;
    initStyleOption(&opt, index);

    auto markdown = index.data(Qt::DisplayRole).toString();
    _document->setMarkdown(markdown, QTextDocument::MarkdownDialectCommonMark);
    _document->setDefaultFont(opt.font);
    _document->setTextWidth(opt.rect.width());

    return QSize(_document->idealWidth(), _document->size().height());
}

QWidget *TreeDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const {
    auto opt = option;
    initStyleOption(&opt, index);

    auto *editor = new QLineEdit(parent);
    editor->setFont(opt.font);

    return editor;
}

void TreeDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const {
    auto *line_edit = qobject_cast<QLineEdit *>(editor);
    line_edit->setText(index.data(Qt::EditRole).toString());
}

void TreeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const {
    auto *line_edit = qobject_cast<QLineEdit *>(editor);
    model->setData(index, line_edit->text());
}

void TreeDelegate::initStyleOption(QStyleOptionViewItem *option,
                                   const QModelIndex &index) const {
    auto font = _settings.value("view/font");
    if (font.isValid()) {
        option->font.fromString(font.toString());
    }

    if (option->state & QStyle::State_Selected) {
        QBrush highlight = option->palette.highlight();
        option->backgroundBrush.swap(highlight);
    }

    auto markdown = index.data(Qt::DisplayRole).toString();
    // https://www.w3.org/TR/SVG11/types.html#ColorKeywords
    if (markdown.startsWith("TODO:")) {
        option->backgroundBrush.setColor(QColor("yellow"));
    } else if (markdown.startsWith("DONE:")) {
        option->backgroundBrush.setColor(QColor("green"));
    } else if (markdown.startsWith("BUG:")) {
        option->backgroundBrush.setColor(QColor("orange"));
    } else if (markdown.startsWith("ETA:")) {
        option->backgroundBrush.setColor(QColor("purple"));
    } else if (markdown.startsWith("P0:")) {
        option->backgroundBrush.setColor(QColor("red"));
    } else if (markdown.startsWith("P1:")) {
        option->backgroundBrush.setColor(QColor("orange"));
    } else if (markdown.startsWith("P2:")) {
        option->backgroundBrush.setColor(QColor("yellow"));
    } else if (markdown.startsWith("P3:")) {
        option->backgroundBrush.setColor(QColor("purple"));
    }
}
