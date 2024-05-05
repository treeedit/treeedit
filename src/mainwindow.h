#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "treedelegate.h"
#include "treefile.h"
#include "treemodel.h"

#include <QMainWindow>
#include <QTreeWidget>
#include <QSortFilterProxyModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_actionOpen_triggered();

  void on_actionAppendChild_triggered();
  void on_actionAppendSibling_triggered();
  void on_actionPrependSibling_triggered();
  void on_actionMoveUp_triggered();
  void on_actionMoveDown_triggered();
  void on_actionPromote_triggered();
  void on_actionRemove_triggered();

  void on_actionNew_triggered();

  void on_actionFont_triggered();

  void on_actionDocs_triggered();

  void on_lineEdit_textChanged(const QString &arg1);

  private:
  Ui::MainWindow *ui;

  TreeFile *_tree_file;
  TreeModel *_tree_model;
  TreeDelegate *_tree_delegate;
  QSortFilterProxyModel *_tree_proxy;
};
#endif // MAINWINDOW_H
