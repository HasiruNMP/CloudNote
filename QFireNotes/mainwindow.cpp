#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);
    netMngNotes = new QNetworkAccessManager(this);

    //connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onListMailItemClicked(QListWidgetItem*)));
    //connect(ui->listWidget->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this,SLOT(enableEditDelete()));
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
        authUser = loginUN;
        ui->lineLoginUN->clear();
        ui->lineLoginPW->clear();
        ui->stackedWidget->setCurrentIndex(3);
        getAllNotes(authUser);
    }
    else
    {
        qDebug() << "invalid username or password";
        ui->lineLoginUN->clear();
        ui->lineLoginPW->clear();
        showAlert("Invalid username or password!");
    }

}

void MainWindow::enableEditDelete()
{
    ui->btnEditNote->setEnabled(true);
    ui->btnDeleteNote->setEnabled(true);
}

void MainWindow::showAlert(QString msg)
{
    QMessageBox::information(this, "QFire Notes", msg);
}

void MainWindow::refresh()
{
    ui->listWidget->clear();
    ui->lineNoteTitle->clear();
    ui->plainTextEdit->clear();
    ui->btnEditNote->setEnabled(false);
    ui->btnDeleteNote->setEnabled(false);
    getAllNotes(authUser);
}

void MainWindow::getAllNotes(QString user)
{
    qDebug() << "getting all notes" << user;

    QString url = "https://qfirenotes-default-rtdb.firebaseio.com/users/" + user + "/notenames.json";
    qDebug() << url << "678";
    netRepGetNotes = netMngNotes->get(QNetworkRequest(QUrl(url)));
    connect(netRepGetNotes, &QNetworkReply::readyRead, this, &MainWindow::readAllNotes );
}


void MainWindow::showNoteList(QStringList newList)
{
    ui->listWidget->clear();
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
    QByteArray noteListQb = netRepGetNotes->readAll();
    qDebug() << noteListQb;

    if(noteListQb != "null"){

        QString noteListStr = QString(noteListQb);
        //qDebug() << noteListStr;
        noteListStr.remove('"');
        noteListStr.remove('[');
        noteListStr.remove(']');
        noteListStr.remove('{');
        noteListStr.remove('}');
        noteListStr.remove('~');
        noteListStr.remove(':');
        //qDebug() << noteListStr;

        QStringList noteNameList = noteListStr.split(",");
        //qDebug() << noteNameList;
        noteList = noteNameList;
        showNoteList(noteNameList);
    }
    else{
        qDebug() << noteListQb << "345";
        showAlert("You don't have any notes yet. Create your first note!");
    }


}

void MainWindow::showNote()
{
    QByteArray showNoteQb = netRepShowNote->readAll();
    QString showNoteStr = QString(showNoteQb);
    showNoteStr.remove('"');

    if(!editOn){
        ui->plainTextEdit->setPlainText(showNoteStr);
    }
    else{
        ui->plainNoteText->setPlainText(showNoteStr);
    }

}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    qDebug() << item->text();
    selectedNote = item->text();
    ui->lineNoteTitle->setText(selectedNote);
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

    QString newtitle = ui->lineAddTitle->text();

    QString url = "https://qfirenotes-default-rtdb.firebaseio.com/users/" + authUser + "/notes/" + newtitle + ".json";

    QVariantMap newNoteVM;
    newNoteVM["content"] = ui->plainNoteText->toPlainText();
    QJsonDocument newNoteJson = QJsonDocument::fromVariant(newNoteVM);

    QNetworkRequest newNoteReq((QUrl(url)));
    newNoteReq.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    netMngNotes->put(newNoteReq, newNoteJson.toJson());

    QString url2 = "https://qfirenotes-default-rtdb.firebaseio.com/users/" + authUser + "/notenames.json";

    QVariantMap uplistVM;

    if(!editOn){
       noteList.append(newtitle);
    }
    else{
        editOn = false;
    }

    for (auto& i : noteList)
    {
        qDebug() << i;
        if(i != "null"){
            uplistVM[i] = "~";
        }
    }

    QJsonDocument upListJson = QJsonDocument::fromVariant(uplistVM);

    QNetworkRequest upListReq((QUrl(url2)));
    upListReq.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    netMngNotes->put(upListReq, upListJson.toJson());

    ui->lineAddTitle->clear();
    ui->plainNoteText->clear();
    ui->stackedWidget->setCurrentIndex(3);

    ui->labelSaveEdit->setText("Create a note");
    ui->lineAddTitle->setReadOnly(false);
    ui->btnSaveNote->setText("Save New Note");

    showAlert("Note Saved!");
    refresh();

}

void MainWindow::editNote()
{
    editOn = true;
    ui->stackedWidget->setCurrentIndex(4);
    ui->labelSaveEdit->setText("Edit your note. (note title can't be changed)");
    ui->lineAddTitle->setReadOnly(true);
    ui->btnSaveNote->setText("Save Edited Note");
    ui->lineAddTitle->setText(selectedNote);
    getNote(selectedNote);
}

void MainWindow::deleteNote()
{
    QString url = "https://qfirenotes-default-rtdb.firebaseio.com/users/" + authUser + "notes/" + selectedNote + ".json";
    netDelNote = netMngNotes->deleteResource(QNetworkRequest(QUrl(url)));

    QVariantMap uplistVM;
    noteList.removeAll(selectedNote);

    for (auto& i : noteList)
    {
        qDebug() << i;
        if(i != "null"){
            uplistVM[i] = "~";
        }
    }

    QString url2 = "https://qfirenotes-default-rtdb.firebaseio.com/users/" + authUser + "/notenames.json";

    QJsonDocument upListJson = QJsonDocument::fromVariant(uplistVM);
    QNetworkRequest upListReq((QUrl(url2)));
    upListReq.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    netMngNotes->put(upListReq, upListJson.toJson());

    showAlert("Note Deleted!");
    refresh();
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
    if(ui->lineLoginUN->text() == "" || ui->lineLoginPW->text() == ""){
        showAlert("Please fill all fields!");
    }
    else{
        loginAuth();
    }

}


void MainWindow::on_btnSaveNote_clicked()
{
    pushNote();
}


void MainWindow::on_btnDeleteNote_clicked()
{
    deleteNote();
}


void MainWindow::on_btnEditNote_clicked()
{
    editNote();
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
    if(editOn){
        editOn = false;
    }
    ui->stackedWidget->setCurrentIndex(3);
    ui->labelSaveEdit->setText("Create a note");
    ui->lineAddTitle->setReadOnly(false);
    ui->btnSaveNote->setText("Save New Note");
}






void MainWindow::on_btnAuthLogout_clicked()
{

}


void MainWindow::on_listWidget_itemSelectionChanged()
{
    enableEditDelete();
}

