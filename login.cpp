#include "login.h"
#include "ui_login.h"
#include <QAction>
#include <QDebug>
#include <Qtxml>
#include <QFile>
#include <QCompleter>       // 自动补全
#include <QListWidget>
#include <map>
#include "accountwifget.h"
#include <QEvent>
#include <QTimer>

struct LoginPrivat
{
    QDomDocument doc;   // xml文件对象
    QString filePath;   // 文件路径
    // QStringList accountList;    // 账号列表
    std::map<QString, QDomNode> accountMap;
    QListWidget accListWidget;
};

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login), p(new LoginPrivat)
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

    ReadFromXML("D:/ProgramData/VScode/C++/QT/MusicPlayer/Config/account.xml");




    connect(ui->lineEdit_account, &QLineEdit::textChanged, this, &Login::on_lineEdit_account_textChanged);
    connect(&p->accListWidget, &QListWidget::itemClicked, this, &Login::CompleteAccount);

    connect(ui->checkBox_remPasswd, &QCheckBox::stateChanged, this, &Login::on_checkBox_remPasswd_stateChanged);
    connect(ui->checkBox_autoLogin, &QCheckBox::stateChanged, this, &Login::on_checkBox_autoLogin_stateChanged);
    
    connect(ui->loginButton, &QPushButton::clicked, [=](){
        QDomElement root = p->doc.documentElement();
        QDomElement type = p->doc.createElement("user");
        QDomElement account1 = p->doc.createElement("账号");
        QDomText accountText = p->doc.createTextNode(ui->lineEdit_account->text());
        type.appendChild(account1);
        account1.appendChild(accountText);
        root.appendChild(account1);
        WriteToXML(p->filePath);
    });

    // 自动补全
#if 0
    QCompleter *completer = new QCompleter(p->accountList, this);
    ui->lineEdit_account->setCompleter(completer);
#endif

    p->accListWidget.setParent(this);
    p->accListWidget.hide();

    qApp->installEventFilter(this);     //将当前界面加入事件过滤器

}

Login::~Login()
{
    delete ui;
    delete p;
}

void Login::on_lineEdit_account_textChanged(const QString &text)
{
    p->accListWidget.clear();

    // 要匹配的文本
    qDebug() << "mapSize:" << p->accountMap.size();
    for(auto it = p->accountMap.begin(); it != p->accountMap.end(); it++)
    {
        QDomNode node = it->second;
        if(it->first.contains(text) && !text.isEmpty())
        {
            AccountWidget *w = new AccountWidget;
            w->setAccount(node.toElement().firstChildElement("账号").text());
            w->setName(node.toElement().firstChildElement("用户名").text());
            w->setPicture(node.toElement().firstChildElement("头像").text());
            QListWidgetItem *item = new QListWidgetItem(&p->accListWidget);
            item->setSizeHint(QSize(ui->lineEdit_account->width() - 20, w->height()));
            p->accListWidget.setItemWidget(item, w);
        }

        if (it->first == text)
        {
            if (node.toElement().attribute("记住密码") == "true")
            {
                ui->checkBox_remPasswd->setChecked(true);
            }
            if (node.toElement().attribute("自动登录") == "true")
            {
                ui->checkBox_autoLogin->setChecked(true);
            }
            if (p->accListWidget.count() == 1)
            {
                p->accListWidget.hide();
            }
        }
    }
    p->accListWidget.move(ui->lineEdit_account->x(), ui->lineEdit_account->y()+ui->lineEdit_account->height());
    p->accListWidget.setFixedWidth(ui->lineEdit_account->width());
    p->accListWidget.setFixedHeight(ui->lineEdit_account->height()*3);

    p->accListWidget.show();

}

bool Login::ReadFromXML(const QString &filename)
{
    p->filePath = filename;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "file open error" + filename;
        return false;
    }
    auto content = file.readAll();
    file.close();

    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    if (!p->doc.setContent(content, &errorMsg, &errorLine, &errorColumn)) 
    {
        qDebug() << "XML 解析错误:" << errorMsg << "at line" << errorLine << "column" << errorColumn;
        return false;
    }
    qDebug() << "xml 解析成功";

    auto root = p->doc.documentElement();
    for(auto userNode = root.firstChild(); userNode.isNull() == false; userNode = userNode.nextSibling())
    {
        qDebug() << "root" ;

        QString account = userNode.toElement().firstChildElement("账号").text();

        p->accountMap.insert({account, userNode});
    }

    return true;;
}

bool Login::eventFilter(QObject *watched, QEvent *event)
{
    if(watched != &p->accListWidget && event->type() == QEvent::MouseButtonPress)
    {
        QTimer::singleShot(100, [&](){p->accListWidget.hide();});   // 延后执行隐藏函数，使clicked信号能正常发出
        // p->accListWidget.hide();
    }
    return QWidget::eventFilter(watched, event);
}

void Login::CompleteAccount()
{
    QListWidgetItem *item = p->accListWidget.currentItem();
    if(item == nullptr)
    {
        return;
    }
    qDebug() << "item:" << item->text();
    QWidget *w = p->accListWidget.itemWidget(item);
    AccountWidget *aw = static_cast<AccountWidget*>(w);
    ui->lineEdit_account->setText(aw->getAccount());
    p->accListWidget.hide();
}

void Login::on_checkBox_remPasswd_stateChanged()
{
    auto it = p->accountMap.find(ui->lineEdit_account->text());
    if(it == p->accountMap.end())
    {
        return;
    }
    QDomNode node = it->second;

    if(ui->checkBox_remPasswd->checkState() == Qt::Checked)
    {
        node.toElement().setAttribute("记住密码", "true");
    }
    else
    {
        ui->checkBox_autoLogin->setChecked(false);
        node.toElement().setAttribute("记住密码", "false");
    }
    WriteToXML(p->filePath);
}
void Login::on_checkBox_autoLogin_stateChanged()
{
    auto it = p->accountMap.find(ui->lineEdit_account->text());
    if(it == p->accountMap.end())
    {
        return;
    }
    QDomNode node = it->second;

    if (ui->checkBox_autoLogin->checkState() == Qt::Checked)
    {
        node.toElement().setAttribute("自动登录", "true");
        ui->checkBox_remPasswd->setChecked(true);
    }
    else
    {
        node.toElement().setAttribute("自动登录", "false");
    }
    WriteToXML(p->filePath);
}
bool Login::WriteToXML(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "file open error" + filename;
        return false;
    }
    if (file.write(p->doc.toByteArray()) <= 0)
    {
        qDebug() << p->doc.toByteArray();
        qDebug() << "file write error" + filename;
        file.close();
        return false;
    }
    file.close();

    return true;
}