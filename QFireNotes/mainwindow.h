#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QListWidgetItem>

#include "notes.h"
#include "addnote.h"

using std::string;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getAllNotes(QString user);
    QStringList noteNameList;

public slots:
    void showNoteList(QStringList newList);
    void readAllNotes();
    void showNote();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager * netMngNotes;
    QNetworkReply * netRepGetNotes;
    QNetworkReply * netRepShowNote;
    QString selectedNote;

    void getNote(QString notename);
};

#endif // MAINWINDOW_H
