#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[]) {
  QCoreApplication::setOrganizationName("Paul Borza");
  QCoreApplication::setOrganizationDomain("treeedit.org");
  QCoreApplication::setApplicationName("Tree Edit");

  QApplication application(argc, argv);
  // application.setStyle("fusion");

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "treeedit_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      application.installTranslator(&translator);
      break;
    }
  }

  MainWindow window;
  window.show();

  return application.exec();
}
