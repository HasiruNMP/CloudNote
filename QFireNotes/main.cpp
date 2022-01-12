#include "mainwindow.h"
#include "addnote.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    w.getAllNotes("");

    //AddNote abc;
    //abc.show();

    return a.exec();
}
