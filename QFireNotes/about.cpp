#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) : QDialog(parent), ui(new Ui::About)
{
    ui->setupUi(this);
    this->setWindowTitle("About CloudNote");
    QPixmap logo1(":/img/res/img/cloudnote.png");
    ui->label->setPixmap(logo1.scaled(80,80,Qt::KeepAspectRatio));
}

About::~About()
{
    delete ui;
}
