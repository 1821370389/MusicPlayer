#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class Login;
}

struct LoginPrivat;

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    bool ReadFromXML(const QString &filename);
    bool WriteToXML(const QString &filename);
    bool eventFilter(QObject *watched, QEvent *event);

public slots:
    void on_lineEdit_account_textChanged(const QString &text);
    void CompleteAccount();
    void on_checkBox_remPasswd_stateChanged();
    void on_checkBox_autoLogin_stateChanged();
    

private:
    Ui::Login *ui;
    LoginPrivat *p;
};

#endif // LOGIN_H
