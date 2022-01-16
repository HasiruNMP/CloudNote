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
    void enableEditDelete();

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
    void on_btnEditNote_clicked();
    void on_btnDeleteNote_clicked();
    void on_btnAuthLogout_clicked();
    void on_listWidget_itemSelectionChanged();
    void on_btnRefresh_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager * netMngNotes;
    QNetworkReply * netRepGetNotes;
    QNetworkReply * netRepShowNote;
    QNetworkReply * netRepValUN;
    QNetworkReply * netRepAuth;
    QNetworkReply * netDelNote;
    QNetworkReply * netRepAuthAuto;
    QString selectedNote;
    QString newUN;
    QString newPW;
    QString newName;
    QString loginUN;
    QString loginPW;
    QString storedUN;
    QString storedPW;
    QString authUser;
    QString authName;
    QStringList noteList;
    bool editOn = false;
    QRegularExpression rx;
    QValidator *validator;


    void getNote(QString notename);
    void pushNote();
    void editNote();
    void deleteNote();
    void validateUsername();
    void createAccount();
    void loginAuth();
    void loginAuthAuto();
    void readLAuthAuto();
    void showAlert(QString msg);
    void refresh();
    void confirmDelete();
    void confirmLogout();
    void logout();
    void readAuthFile();
    void setAuthFile(QStringList authstrlist);
    bool checkTitle(QString title);
};

#endif // MAINWINDOW_H
