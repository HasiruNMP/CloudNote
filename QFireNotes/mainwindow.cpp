#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addnote.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(3);
    netMngNotes = new QNetworkAccessManager(this);
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onListMailItemClicked(QListWidgetItem*)));

}

MainWindow::~MainWindow()
{
    delete ui;
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
    QString urlp5 = ".json";
    QString url = urlp1+urlp2+urlp3+urlp4+urlp5;

    qDebug() << url;
    netRepShowNote = netMngNotes->get(QNetworkRequest(QUrl(url)));
    connect(netRepShowNote, &QNetworkReply::readyRead, this, &MainWindow::showNote);
}

void MainWindow::on_pushButton_clicked()
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
    //netRepShowNote = netMngNotes->deleteResource(QNetworkRequest(QUrl(url)));
}


void MainWindow::on_pushButton_2_clicked()
{

}

