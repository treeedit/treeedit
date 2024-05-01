#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAbstractItemModelTester>
#include <QDesktopServices>
#include <QDir>
#include <QErrorMessage>
#include <QFileDialog>
#include <QFontDialog>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setWindowIcon(QIcon(":/icons/treeedit.icns"));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionNew_triggered() {
    _tree_delegate = new TreeDelegate;
    ui->treeView->setItemDelegate(_tree_delegate);

    auto path = QFileDialog::getSaveFileName(
        this, tr("Save File"), QDir::homePath(), tr("Tree Edit (*.db)"));

    _tree_file = new TreeFile(path);
    if (!_tree_file->create()) {
        QErrorMessage error(this);
        error.showMessage("Failed to create new file.");
        error.exec();
    }

    _tree_proxy = new QSortFilterProxyModel;
    _tree_proxy->setRecursiveFilteringEnabled(true);
    _tree_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

    _tree_model = new TreeModel(_tree_file);
    _tree_proxy->setSourceModel(_tree_model);

    ui->treeView->setModel(_tree_proxy);
}

void MainWindow::on_actionOpen_triggered() {
    _tree_delegate = new TreeDelegate;
    ui->treeView->setItemDelegate(_tree_delegate);

    auto path = QFileDialog::getOpenFileName(
        this, tr("Open File"), QDir::homePath(), tr("Tree Edit (*.db)"));

    _tree_file = new TreeFile(path);

    _tree_proxy = new QSortFilterProxyModel;
    _tree_proxy->setRecursiveFilteringEnabled(true);
    _tree_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

    _tree_model = new TreeModel(_tree_file);
    _tree_proxy->setSourceModel(_tree_model);

    ui->treeView->setModel(_tree_proxy);

#ifdef QT_DEBUG
    // https://wiki.qt.io/Model_Test
    new QAbstractItemModelTester(
        _tree_model, QAbstractItemModelTester::FailureReportingMode::Fatal, this);
#endif
}

void MainWindow::on_actionAppendChild_triggered() {
    if (_tree_model) {
        auto index = ui->treeView->currentIndex();
        auto last = _tree_model->appendChild(index);

        if (last.isValid()) {

            // Scrolls into view, selects, and edits.
            ui->treeView->scrollTo(last);
            ui->treeView->setCurrentIndex(last);
            ui->treeView->edit(last);
        }
    }
}

void MainWindow::on_actionAppendSibling_triggered() {
    if (_tree_model) {
        auto index = ui->treeView->currentIndex();
        auto last = _tree_model->appendSibling(index);

        if (last.isValid()) {

            // Scrolls into view, selects, and edits.
            ui->treeView->scrollTo(last);
            ui->treeView->setCurrentIndex(last);
            ui->treeView->edit(last);
        }
    }
}

void MainWindow::on_actionPrependSibling_triggered() {
    if (_tree_model) {
        auto index = ui->treeView->currentIndex();
        auto last = _tree_model->prependSibling(index);

        if (last.isValid()) {

            // Scrolls into view, selects, and edits.
            ui->treeView->scrollTo(last);
            ui->treeView->setCurrentIndex(last);
            ui->treeView->edit(last);
        }
    }
}

void MainWindow::on_actionMoveUp_triggered() {
    if (_tree_model) {
        auto index = ui->treeView->currentIndex();
        _tree_model->moveItem(index, -1);
    }
}

void MainWindow::on_actionMoveDown_triggered() {
    if (_tree_model) {
        auto index = ui->treeView->currentIndex();
        _tree_model->moveItem(index, +1);
    }
}

void MainWindow::on_actionRemove_triggered() {
    if (_tree_model) {
        auto index = ui->treeView->currentIndex();
        _tree_model->removeItem(index);
    }
}

void MainWindow::on_actionFont_triggered() {
    QSettings settings;
    bool ok;
    auto font = QFontDialog::getFont(&ok, this);
    if (ok) {
        settings.setValue("view/font", font.toString());
    }
}

void MainWindow::on_actionDocs_triggered() {
    QDesktopServices::openUrl(QUrl("https://treeedit.org"));
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1) {
    if (_tree_proxy) {
        _tree_proxy->setFilterFixedString(arg1);
    }
}
