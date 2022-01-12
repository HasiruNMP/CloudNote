#ifndef ADDNOTE_H
#define ADDNOTE_H
#include "mainwindow.h"
#include <QWidget>

namespace Ui {
class AddNote;
}

class AddNote : public QWidget
{
    Q_OBJECT

public:
    explicit AddNote(QWidget *parent = nullptr);
    ~AddNote();
    void pushNewNote();

private slots:
    void on_pushButton_clicked();

private:
    Ui::AddNote *ui;
    QNetworkAccessManager * netMngNewNote;
    QNetworkReply * netRepAddNote;
};

#endif // ADDNOTE_H
