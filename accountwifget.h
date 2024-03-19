#ifndef ACCOUNTWIFGET_H
#define ACCOUNTWIFGET_H

#include <QWidget>

namespace Ui {
class AccountWidget;
}

class AccountWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AccountWidget(QWidget *parent = nullptr);
    ~AccountWidget();
    void setAccount(const QString& account);
    void setName(const QString& name);
    void setPicture(const QString& picture);
    QString getAccount();
private:
    Ui::AccountWidget *ui;
};

#endif // ACCOUNTWIFGET_H
