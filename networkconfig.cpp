#include "networkconfig.h"
#include "./ui_networkconfig.h"
#include <QFile>
#include <QDomDocument>
#include <QDebug>


struct configprivate
{
    QDomDocument doc;   // xml文件对象
    QString filePath;   // 文件路径
};


NetworkConfig::NetworkConfig(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NetworkConfig), p(new configprivate)
{
    ui->setupUi(this);
    connect(ui->pushButton_OK, &QPushButton::clicked, this, &NetworkConfig::accept);
    connect(ui->pushButton_Cancel, &QPushButton::clicked, this, &NetworkConfig::reject);
}

NetworkConfig::~NetworkConfig()
{
    delete ui;
    delete p;
}

bool NetworkConfig::ReadFromXML(const QString &filename)
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
    for(auto childNode = root.firstChild(); childNode.isNull() == false; childNode = childNode.nextSibling())
    {
        qDebug() << childNode.nodeName();
        qDebug() << childNode.nodeValue();
        ui->lineEdit_ip->setText(root.firstChildElement("ip").text());
        ui->lineEdit_port->setText(root.firstChildElement("port").text());
    }

    return true;
}
bool NetworkConfig::WriteToXML(const QString &filename)
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
        return false;
    }
    file.close();

    return true;
}

std::pair<QString, int> NetworkConfig::GetNetWorkConfig()
{
    qDebug() << ui->lineEdit_ip->text();
    qDebug() << ui->lineEdit_port->text();
    return std::pair<QString, int>(ui->lineEdit_ip->text(), ui->lineEdit_port->text().toInt());
}

void NetworkConfig::accept()
{
    auto root = p->doc.documentElement();
    root.firstChildElement("ip").firstChild().setNodeValue(ui->lineEdit_ip->text());
    root.firstChildElement("port").firstChild().setNodeValue(ui->lineEdit_port->text());
    WriteToXML(p->filePath);
    this->close();
}
void NetworkConfig::reject()
{
    this->close();
}
