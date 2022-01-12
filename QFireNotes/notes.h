#ifndef NOTES_H
#define NOTES_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>

using std::string;

class Notes : public QObject
{
    Q_OBJECT
public:
    explicit Notes(QObject *parent = nullptr);
    void getAllNotes(QString user);
    QStringList noteNameList;

signals:
    void emitAllNotes(QStringList emitlist);

private:
    QNetworkAccessManager * netMngNotes;
    QNetworkReply * netRepGetNotes;

public slots:
    void readAllNotes();
};

#endif // NOTES_H
