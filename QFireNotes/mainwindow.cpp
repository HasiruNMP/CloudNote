#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);
    netMngNotes = new QNetworkAccessManager(this);
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onListMailItemClicked(QListWidgetItem*)));

}

MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::validateUsername()
{
    newUN = ui->lineSignupUN->text();
    newPW = ui->lineSignupPW->text();

    QString url = "https://qfirenotes-default-rtdb.firebaseio.com/users/" + newUN + "/auth/username.json";
    //qDebug() << url;

    netRepValUN = netMngNotes->get(QNetworkRequest(QUrl(url)));
    connect(netRepValUN, &QNetworkReply::readyRead, this, &MainWindow::readUNVali);
}

void MainWindow::readUNVali()
{
    QByteArray valResult = netRepValUN->readAll();
    qDebug() << valResult << "123";
    if(valResult == "null"){
        qDebug() << "Valid";
        createAccount();
    }
    else
    {
        qDebug() << "Not Valid";
        showAlert("Someone already uses that username! Please enter a new username");
    }
}

void MainWindow::createAccount()
{
    QVariantMap newAccVM;
    newAccVM["username"] = newUN;
    newAccVM["password"] = newPW;
    QJsonDocument newAccJson = QJsonDocument::fromVariant(newAccVM);

    QString url = "https://qfirenotes-default-rtdb.firebaseio.com/users/" + newUN + "/auth.json";
    qDebug() << url;

    QNetworkRequest newAccReq((QUrl(url)));
    newAccReq.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    netMngNotes->put(newAccReq, newAccJson.toJson());

    ui->lineSignupUN->clear();
    ui->lineSignupPW->clear();
    ui->lineSignupPWRe->clear();
    showAlert("Registration Succesfull! Login in the next screen");
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::loginAuth()
{
    loginUN = ui->lineLoginUN->text();
    loginPW = ui->lineLoginPW->text();

    QString url = "https://qfirenotes-default-rtdb.firebaseio.com/users/" + loginUN + "/auth.json";
    netRepAuth = netMngNotes->get(QNetworkRequest(QUrl(url)));
    connect(netRepAuth, &QNetworkReply::readyRead, this, &MainWindow::readLAuth);
}

void MainWindow::readLAuth()
{
    QByteArray authResult = netRepAuth->readAll();
    qDebug() << authResult << "456";
    QString auth = QString(authResult);
    auth.remove('"');
    auth.remove('{');
    auth.remove('}');
    auth.remove('\\');
    QRegularExpression separator("[(,|:|)]");
    QStringList unamepw = auth.split(separator);
    qDebug() << unamepw;
    if(loginUN == unamepw[3] && loginPW == unamepw[1])
    {
        qDebug() << "234";
        showAlert("Login Succesfull!");
        ui->lineLoginUN->clear();
        ui->lineLoginPW->clear();
        ui->stackedWidget->setCurrentIndex(3);
    }
    else
    {
        qDebug() << "invalid username or password";
        ui->lineLoginUN->clear();
        ui->lineLoginPW->clear();
        showAlert("Invalid username or password!");
    }

}

void MainWindow::showAlert(QString msg)
{
    QMessageBox::information(this, "QFire Notes", msg);
}

void MainWindow::getAllNotes(QString user)
{
    qDebug() << "getting all notes" << user;

    netRepGetNotes = netMngNotes->get(QNetworkRequest(QUrl("https://qfirenotes-default-rtdb.firebaseio.com/users/hasiru/notenames.json")));
    connect(netRepGetNotes, &QNetworkReply::readyRead, this, &MainWindow::readAllNotes );

    qDebug() << "got all notes";
}


void MainWindow::showNoteList(QStringList newList)
{
    for (auto& i : newList)
    {
        qDebug() << i;
        if(i != "null"){
            ui->listWidget->addItem(i);
            //ui->listWidget->addItem("1234");
        }
    }
}

void MainWindow::readAllNotes()
{
    //qDebug() << netRepGetNotes->readAll();

    QByteArray namelistbarr = netRepGetNotes->readAll();
    QString nameliststr = QString(namelistbarr);
    qDebug() << nameliststr;
    nameliststr.remove('"');
    nameliststr.remove('[');
    nameliststr.remove(']');
    qDebug() << nameliststr;

    QStringList noteNameList = nameliststr.split(",");
    qDebug() << noteNameList;

    //MainWindow test;
    showNoteList(noteNameList);

}

void MainWindow::showNote()
{
    QByteArray namelistbarr = netRepShowNote->readAll();
    QString nameliststr = QString(namelistbarr);
    qDebug() << nameliststr;
    nameliststr.remove('"');
    nameliststr.remove('[');
    nameliststr.remove(']');
    qDebug() << nameliststr;

    QStringList newNote = nameliststr.split(",");
    qDebug() << newNote;
    ui->plainTextEdit->setPlainText(nameliststr);
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    qDebug() << item->text();
    selectedNote = item->text();
    getNote(selectedNote);
}

void MainWindow::getNote(QString notename)
{
    QString urlp1 = "https://qfirenotes-default-rtdb.firebaseio.com/users/";
    QString urlp2 = "hasiru/";
    QString urlp3 = "notes/";
    QString urlp4 = notename;
    QString urlp5 = "/content.json";
    QString url = urlp1+urlp2+urlp3+urlp4+urlp5;

    qDebug() << url;
    netRepShowNote = netMngNotes->get(QNetworkRequest(QUrl(url)));
    connect(netRepShowNote, &QNetworkReply::readyRead, this, &MainWindow::showNote);
}

void MainWindow::pushNote()
{
    QString urlp1 = "https://qfirenotes-default-rtdb.firebaseio.com/users/";
    QString urlp2 = "hasiru/";
    QString urlp3 = "notes/";
    QString urlp4 = ui->lineAddTitle->text();
    QString urlp5 = ".json";
    QString url = urlp1+urlp2+urlp3+urlp4+urlp5;

    //QString newMsgText = ui->msgLineEdit->text();
    QVariantMap newNoteVM;
    newNoteVM["content"] = ui->plainNoteText->toPlainText();
    QJsonDocument newNoteJson = QJsonDocument::fromVariant(newNoteVM);

    QNetworkRequest newNoteReq((QUrl(url)));
    newNoteReq.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    netMngNotes->put(newNoteReq, newNoteJson.toJson());

    ui->lineAddTitle->clear();
    ui->plainNoteText->clear();
    ui->stackedWidget->setCurrentIndex(3);

}

void MainWindow::deleteNote()
{
    QString urlp1 = "https://qfirenotes-default-rtdb.firebaseio.com/users/";
    QString urlp2 = "hasiru/";
    QString urlp3 = "notes/";
    QString urlp4 = selectedNote;
    QString urlp5 = ".json";
    QString url = urlp1+urlp2+urlp3+urlp4+urlp5;

    qDebug() << url;
    auto myvar1 = QDateTime::currentDateTime().time();
    qDebug() << myvar1;
    netRepShowNote = netMngNotes->deleteResource(QNetworkRequest(QUrl(url)));
}

//funtion buttons
void MainWindow::on_btnSignupCreate_clicked()
{
    if( ui->lineSignupUN->text()=="" || ui->lineSignupPW->text()=="" || ui->lineSignupPWRe->text()=="" ){
        showAlert("Please fill all the details");
    }
    else if(ui->lineSignupPW->text() != ui->lineSignupPWRe->text())
    {
        showAlert("passwords don't match");
    }
    else{
        validateUsername();
    }
}

void MainWindow::on_btnLogin_clicked()
{
    loginAuth();
}

void MainWindow::on_btnSaveNote_clicked()
{
    pushNote();
}


// navigation buttons slots
void MainWindow::on_btnNavLogin_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_btnNavSignup_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_btnCancelLogin_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_btnCancelSignup_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_btnNavCreate_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_btnCancelSave_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}







