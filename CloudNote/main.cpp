#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //setting the application icon
    a.setWindowIcon(QIcon(":/img/res/img/appicon.ico"));

    //setting the application stylesheet
    QFile stylefile(":/style/res/style/cloudnotestyle.qss");
    stylefile.open(QFile::ReadOnly);
    QString stylesheet = QLatin1String(stylefile.readAll());
    a.setStyleSheet(stylesheet);

    //starting the main window
    MainWindow w;
    w.setWindowTitle("CloudNote");
    w.show();

    return a.exec();
}
