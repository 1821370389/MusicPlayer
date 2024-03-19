#include "accountwifget.h"
#include "ui_accountwifget.h"

AccountWidget::AccountWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AccountWidget)
{
    ui->setupUi(this);
}

AccountWidget::~AccountWidget()
{
    delete ui;
}

void AccountWidget::setAccount(const QString& account)
{
    qDebug() << account;
    ui->label_account->setText(account);
}

void AccountWidget::setName(const QString& name)
{
    ui->label_name->setText(name);
}

void AccountWidget::setPicture(const QString& picture)
{
    ui->label_picture->setPixmap(QPixmap(picture).scaled(this->width()/3, this->height()));
}
QString AccountWidget::getAccount()
{
    
    return ui->label_account->text();
}