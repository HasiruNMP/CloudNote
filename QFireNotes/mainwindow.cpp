#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QRegularExpression>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    netMngNotes = new QNetworkAccessManager(this);

    QMovie *movie = new QMovie(":/img/res/img/loading.gif");

    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(5);
    ui->labelGif->setMovie(movie);
    movie->start();


    QPixmap logo1(":/img/res/img/cloudnote.png");
    ui->labelLogo->setPixmap(logo1.scaled(250,250,Qt::KeepAspectRatio));
    ui->labelLogin->setPixmap(logo1.scaled(120,120,Qt::KeepAspectRatio));
    ui->labelSignup->setPixmap(logo1.scaled(120,120,Qt::KeepAspectRatio));
    ui->labelLogo2->setPixmap(logo1.scaled(120,120,Qt::KeepAspectRatio));

    QPixmap logo2(":/img/res/img/applogo.png");
    ui->noteLogo->setPixmap(logo2.scaled(28,28,Qt::KeepAspectRatio));


    QRegularExpression rxUN("[A-Za-z0-9]{4,20}");
    QValidator *valiUN = new QRegularExpressionValidator(rxUN, this);

    QRegularExpression rxPW("[A-Za-z0-9]{6,20}");
    QValidator *valiPW = new QRegularExpressionValidator(rxPW, this);

    QRegularExpression rxTitle("[A-Za-z0-9\\s]{1,100}");
    QValidator *valiTitle = new QRegularExpressionValidator(rxTitle, this);

    ui->lineLoginUN->setValidator(valiUN);
    ui->lineLoginPW->setValidator(valiPW);
    ui->lineSignupUN->setValidator(valiUN);
    ui->lineSignupPW->setValidator(valiPW);
    ui->lineSignupPWRe->setValidator(valiPW);
    ui->lineAddTitle->setValidator(valiTitle);

    readAuthFile();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readAuthFile()
{
    qDebug() << "890";

    QStringList authList;

    QFile authFile("authuser.txt");
    if (authFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        while (!authFile.atEnd()) authList.append(authFile.readLine());
    }
    authFile.close();

    //qDebug() << authList[0] << authList[1] << authList[2];

    if(!authList.isEmpty())
    {
        if(authList[0]=="loggedin\n")
        {
            storedUN = authList[1].remove("\n");
            qDebug() << authList[1].remove("\n");
            storedPW = authList[2].remove("\n");
            qDebug() << authList[2].remove("\n");
            loginAuthAuto();
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(0);
        }
    }
    else
    {
       ui->stackedWidget->setCurrentIndex(0);
    }




}

void MainWindow::setAuthFile(QStringList authstrlist)
{
    QFile newauthfile("authuser.txt");
    if (newauthfile.open(QIODevice::ReadWrite)) {
        QTextStream stream(&newauthfile);
        stream << authstrlist[0] << "\n";
        stream << authstrlist[1] << "\n";
        stream << authstrlist[2] << "\n";
    }
    newauthfile.close();
    ui->stackedWidget->setCurrentIndex(0);
}

bool MainWindow::checkTitle(QString title)
{
    for (auto& i : noteList)
    {
        qDebug() << i << "note";
        if(i == title){
            return false;
            break;
        }
    }
    return true;
}

void MainWindow::validateUsername()
{
    newUN = ui->lineSignupUN->text();
    newPW = ui->lineSignupPW->text();
    newName = ui->lineSignupName->text();

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
    newAccVM["name"] = newName;
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
    if(authResult != "null")
    {
        QString auth = QString(authResult);
        auth.remove('"');
        auth.remove('{');
        auth.remove('}');
        auth.remove('\\');
        QRegularExpression separator("[(,|:|)]");
        QStringList unamepw = auth.split(separator);
        qDebug() << unamepw;
        if(loginUN == unamepw[5] && loginPW == unamepw[3])
        {
            qDebug() << "234";
            authUser = loginUN;
            authName = unamepw[1];
            QStringList auth = {"loggedin",loginUN,loginPW};
            setAuthFile(auth);
            showAlert("Greetings " + authName + "! \nLogin Succesfull.");
            ui->lineLoginUN->clear();
            ui->lineLoginPW->clear();
            //ui->stackedWidget->setCurrentIndex(3);
            //getAllNotes(authUser);
            //ui->labelUsersNotes->setText(authName + "'s Notes");
        }
        else
        {
            qDebug() << "invalid username or password";
            ui->lineLoginUN->clear();
            ui->lineLoginPW->clear();
            showAlert("Invalid username or password!");
        }
    }
    else
    {
        qDebug() << "invalid username";
        ui->lineLoginUN->clear();
        ui->lineLoginPW->clear();
        showAlert("That username doesn't exist!");
    }


}

void MainWindow::loginAuthAuto()
{
    QString url = "https://qfirenotes-default-rtdb.firebaseio.com/users/" + storedUN + "/auth.json";
    netRepAuthAuto = netMngNotes->get(QNetworkRequest(QUrl(url)));
    connect(netRepAuthAuto, &QNetworkReply::readyRead, this, &MainWindow::readLAuthAuto);

}

void MainWindow::readLAuthAuto()
{
    QByteArray authResult = netRepAuthAuto->readAll();
    //qDebug() << authResult << "456";
    QString auth = QString(authResult);
    auth.remove('"');
    auth.remove('{');
    auth.remove('}');
    auth.remove('\\');
    QRegularExpression separator("[(,|:|)]");
    QStringList unamepw = auth.split(separator);
    //qDebug() << unamepw;
    if(storedUN == unamepw[5] && storedPW == unamepw[3])
    {
        //qDebug() << "234";
        authUser = storedUN;
        authName = unamepw[1];
        ui->stackedWidget->setCurrentIndex(3);
        getAllNotes(authUser);
        ui->labelUsersNotes->setText(authName + "'s Notes");
    }
    else
    {
        //qDebug() << "invalid username or password";
        ui->stackedWidget->setCurrentIndex(0);
    }

}

void MainWindow::enableEditDelete()
{
    ui->btnEditNote->setEnabled(true);
    ui->btnDeleteNote->setEnabled(true);
}

void MainWindow::showAlert(QString msg)
{
    QMessageBox::information(this, "CloudNote", msg);
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

void MainWindow::confirmDelete()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Confirm Delete", "Are you sure you want to delete the selected note?", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes){
        deleteNote();
    }
}

void MainWindow::confirmLogout()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Confirm Logout", "Are you sure you want to logout?", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes){
        logout();
    }
}

void MainWindow::logout()
{
    qDebug() << "logging out";
    QStringList lout = {"loggedout","null","null"};
    ui->stackedWidget->setCurrentIndex(5);
    ui->listWidget->clear();

    /*for (int i = 0; i < noteList.size(); ++i)
    {
        qDebug() << noteList[i] << "before";
        noteList[i] = "null";
        qDebug() << noteList[i] << "after";
    }*/
    noteList.clear();
    setAuthFile(lout);
}



void MainWindow::getAllNotes(QString user)
{
    //qDebug() << "getting all notes" << user;

    QString url = "https://qfirenotes-default-rtdb.firebaseio.com/users/" + user + "/notenames.json";
    //qDebug() << url << "678";
    netRepGetNotes = netMngNotes->get(QNetworkRequest(QUrl(url)));
    connect(netRepGetNotes, &QNetworkReply::readyRead, this, &MainWindow::readAllNotes );
}


void MainWindow::showNoteList()
{
    ui->listWidget->clear();
    for (auto& i : noteList)
    {
        //qDebug() << i;
        if(i != "null"){
            ui->listWidget->addItem(i);
            //ui->listWidget->addItem("1234");
        }
    }

}

void MainWindow::readAllNotes()
{
    QByteArray noteListQb = netRepGetNotes->readAll();
    //qDebug() << noteListQb;

    if(noteListQb != "null"){

        QString noteListStr = QString(noteListQb);
        qDebug() << noteListStr;
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
        showNoteList();
    }
    else{
        //qDebug() << noteListQb << "345";
        showAlert("You don't have any notes yet. Create your first note!");
    }


}

void MainWindow::showNote()
{
    QByteArray showNoteQb = netRepShowNote->readAll();
    QString showNoteStr = QString(showNoteQb);
    qDebug() << showNoteStr;
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
    //qDebug() << item->text();
    selectedNote = item->text();
    ui->lineNoteTitle->setText(selectedNote);
    getNote(selectedNote);
}

void MainWindow::getNote(QString notename)
{

    QString url = "https://qfirenotes-default-rtdb.firebaseio.com/users/" + authUser + "/notes/" + notename + "/content.json";

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
        //qDebug() << i;
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

    ui->labelSaveEdit->setText("Create New Note");
    ui->lineAddTitle->setReadOnly(false);
    ui->btnSaveNote->setText("SAVE");

    showAlert("Note Saved!");
    refresh();

}

void MainWindow::editNote()
{
    editOn = true;
    ui->stackedWidget->setCurrentIndex(4);
    ui->labelSaveEdit->setText("Edit Your Note. (note title can't be changed)");
    ui->lineAddTitle->setReadOnly(true);
    ui->btnSaveNote->setText("SAVE");
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
        //qDebug() << i;
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
    else if(!(ui->lineSignupUN->text().length() >= 4 && ui->lineSignupPW->text().length() >= 6))
    {
        showAlert("Username lenth should be 4-20 \n Password length should be 6-20");
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

    if(ui->lineAddTitle->text().length() > 0 && ui->plainNoteText->toPlainText().length() > 0){

        if(editOn){
            pushNote();
        }
        else{
            if(checkTitle(ui->lineAddTitle->text()))
            {
                pushNote();
                qDebug() << "Note title is unique";

            }
            else
            {
                showAlert("You already have a note by that title! Enter a different title.");
            }
        }

        //qDebug() << "valid title";
    }
    else
    {
        showAlert("Please enter all fields");
        //qDebug() << "invalid title";
    }
    //
}


void MainWindow::on_btnDeleteNote_clicked()
{
    confirmDelete();
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
    ui->labelSaveEdit->setText("Create New Note");
    ui->lineAddTitle->setReadOnly(false);
    ui->btnSaveNote->setText("SAVE");
}






void MainWindow::on_btnAuthLogout_clicked()
{
    confirmLogout();
}


void MainWindow::on_listWidget_itemSelectionChanged()
{
    enableEditDelete();
}



void MainWindow::on_btnRefresh_clicked()
{
    refresh();
}

