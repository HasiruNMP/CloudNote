#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile stylefile("C:/Work/SoftwareDev/GitHub/QFireNotes/QFireNotes/style/Combinear.qss");
    stylefile.open(QFile::ReadOnly);
    QString stylesheet = QLatin1String(stylefile.readAll());
    a.setStyleSheet(stylesheet);

    MainWindow w;
    w.show();
    //w.getAllNotes("");

    //AddNote abc;
    //abc.show();

    return a.exec();
}
