#include "notes.h"
#include "mainwindow.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <algorithm>

Notes::Notes(QObject *parent) : QObject(parent)
{
    //qDebug() << "got all notes";
    netMngNotes = new QNetworkAccessManager(this);
}

void Notes::getAllNotes(QString user)
{
    qDebug() << "getting all notes" << user;

    netRepGetNotes = netMngNotes->get(QNetworkRequest(QUrl("https://qfirenotes-default-rtdb.firebaseio.com/users/hasiru/notenames.json")));
    connect(netRepGetNotes, &QNetworkReply::readyRead, this, &Notes::readAllNotes );

    qDebug() << "got all notes";
}

void Notes::readAllNotes()
{
    //qDebug() << netRepGetNotes->readAll();

    QByteArray namelistbarr = netRepGetNotes->readAll();
    QString nameliststr = QString(namelistbarr);
    qDebug() << nameliststr;
    nameliststr.erase(std::remove(nameliststr.begin(), nameliststr.end(), '"'), nameliststr.end());
    nameliststr.erase(std::remove(nameliststr.begin(), nameliststr.end(), '['), nameliststr.end());
    nameliststr.erase(std::remove(nameliststr.begin(), nameliststr.end(), ']'), nameliststr.end());
    qDebug() << nameliststr;

    QStringList noteNameList = nameliststr.split(",");
    qDebug() << noteNameList;

    //MainWindow test;
    //showNoteList(noteNameList);

}

