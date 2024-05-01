#include "treedelegate.h"

#include <QLineEdit>
#include <QPainter>

TreeDelegate::TreeDelegate() : _textedit(std::make_unique<QTextEdit>()) {}

void TreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    if (index.column() == 0) {
        auto markdown = index.data(Qt::DisplayRole).toString();
        _textedit->setMarkdown(markdown);
        _textedit->setFont(opt.font);
        _textedit->setPalette(opt.palette);
        _textedit->setFixedWidth(opt.rect.width());
        _textedit->setFixedHeight(opt.rect.height());
        _textedit->setReadOnly(true);

        // Hides the scrollbars.
        _textedit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _textedit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        painter->save();
        painter->translate(opt.rect.topLeft());

        _textedit->render(painter);

        painter->restore();
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize TreeDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    if (index.column() == 0) {
        auto markdown = index.data(Qt::DisplayRole).toString();
        _textedit->setMarkdown(markdown);
        _textedit->setFont(opt.font);
        _textedit->setPalette(opt.palette);
        _textedit->document()->setTextWidth(opt.rect.width());

        return QSize(_textedit->document()->idealWidth(),
                     _textedit->document()->size().height());
    } else {
        return QStyledItemDelegate::sizeHint(option, index);
    }
}

QWidget *TreeDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    if (index.column() == 0) {
        auto *lineedit = new QLineEdit(parent);
        lineedit->setFont(opt.font);

        return lineedit;
    } else {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void TreeDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const {
    if (index.column() == 0) {
        auto *lineedit = qobject_cast<QLineEdit *>(editor);
        lineedit->setText(index.data(Qt::EditRole).toString());
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void TreeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const {
    if (index.column() == 0) {
        auto *lineedit = qobject_cast<QLineEdit *>(editor);
        model->setData(index, lineedit->text());
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void TreeDelegate::initStyleOption(QStyleOptionViewItem *option,
                                   const QModelIndex &index) const {
    if (option->state & QStyle::State_Selected) {
        option->palette.setBrush(QPalette::Base, option->palette.highlight());
        option->palette.setBrush(QPalette::Text, option->palette.highlightedText());
    } else {
        // https://www.w3.org/TR/SVG11/types.html#ColorKeywords
        auto markdown = index.data(Qt::DisplayRole).toString();
        if (markdown.startsWith("TODO:")) {
            option->palette.setBrush(QPalette::Base, QColor("yellow"));
            option->palette.setBrush(QPalette::Text, QColor("black"));
        } else if (markdown.startsWith("DONE:")) {
            option->palette.setBrush(QPalette::Base, QColor("green"));
            option->palette.setBrush(QPalette::Text, QColor("white"));
        } else if (markdown.startsWith("BUG:")) {
            option->palette.setBrush(QPalette::Base, QColor("orange"));
            option->palette.setBrush(QPalette::Text, QColor("black"));
        } else if (markdown.startsWith("ASK:")) {
            option->palette.setBrush(QPalette::Base, QColor("purple"));
            option->palette.setBrush(QPalette::Text, QColor("white"));
        } else if (markdown.startsWith("P0:")) {
            option->palette.setBrush(QPalette::Base, QColor("red"));
            option->palette.setBrush(QPalette::Text, QColor("white"));
        } else if (markdown.startsWith("P1:")) {
            option->palette.setBrush(QPalette::Base, QColor("orange"));
            option->palette.setBrush(QPalette::Text, QColor("black"));
        } else if (markdown.startsWith("P2:")) {
            option->palette.setBrush(QPalette::Base, QColor("yellow"));
            option->palette.setBrush(QPalette::Text, QColor("black"));
        } else if (markdown.startsWith("P3:")) {
            option->palette.setBrush(QPalette::Base, QColor("purple"));
            option->palette.setBrush(QPalette::Text, QColor("white"));
        }
    }
}
