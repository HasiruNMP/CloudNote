#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QListWidgetItem>
#include <QtNetwork>


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
    void readUNVali();
    void readLAuth();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_btnNavLogin_clicked();
    void on_btnNavSignup_clicked();
    void on_btnCancelLogin_clicked();
    void on_btnCancelSignup_clicked();
    void on_btnNavCreate_clicked();
    void on_btnCancelSave_clicked();
    void on_btnSignupCreate_clicked();
    void on_btnSaveNote_clicked();

    void on_btnLogin_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager * netMngNotes;
    QNetworkReply * netRepGetNotes;
    QNetworkReply * netRepShowNote;
    QNetworkReply * netRepValUN;
    QNetworkReply * netRepAuth;
    QString selectedNote;
    QString newUN;
    QString newPW;
    QString loginUN;
    QString loginPW;

    void getNote(QString notename);
    void pushNote();
    void deleteNote();
    void validateUsername();
    void createAccount();
    void loginAuth();
    void showAlert(QString msg);
};

#endif // MAINWINDOW_H
