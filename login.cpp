#include "login.h"
#include "ui_login.h"
#include <QAction>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    QAction *qq = new QAction;
    qq->setIcon(QIcon(":/qq.png"));
    ui->lineEdit_account->addAction(qq, QLineEdit::LeadingPosition);

    QAction *passwd = new QAction;
    passwd->setIcon(QIcon(":/passwd.png"));
    ui->lineEdit_password->addAction(passwd, QLineEdit::LeadingPosition);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);

    ui->loginButton->setObjectName("login");
    ui->pushButton_get_password->setObjectName("find");

}

Login::~Login()
{
    delete ui;
}
