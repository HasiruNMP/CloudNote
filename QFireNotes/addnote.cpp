#include "addnote.h"
#include "ui_addnote.h"

#include <QJsonDocument>
#include <QVariantMap>

AddNote::AddNote(QWidget *parent) : QWidget(parent), ui(new Ui::AddNote)
{
    ui->setupUi(this);
    netMngNewNote = new QNetworkAccessManager();
}

AddNote::~AddNote()
{
    delete ui;
}

void AddNote::pushNewNote()
{
    QString urlp1 = "https://qfirenotes-default-rtdb.firebaseio.com/users/";
    QString urlp2 = "hasiru/";
    QString urlp3 = "notes";
    QString urlp4 = ".json";
    QString url = urlp1+urlp2+urlp3+urlp4;

    //QString newMsgText = ui->msgLineEdit->text();
    QVariantMap newMsgVM;
    newMsgVM[ui->lineEdit->text()] = ui->plainTextEdit->toPlainText();
    QJsonDocument newMsgJson = QJsonDocument::fromVariant(newMsgVM);

    QNetworkRequest newMsgReq((QUrl(url)));
    newMsgReq.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    netMngNewNote->post(newMsgReq, newMsgJson.toJson());

    //ui->msgLineEdit->setText("");
    //getMsg();
}


void AddNote::on_pushButton_clicked()
{
    pushNewNote();
}

