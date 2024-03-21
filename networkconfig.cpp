#include "networkconfig.h"
#include "./ui_networkconfig.h"
#include "xml.h"
#include <QFile>
#include <QDomDocument>
#include <QDebug>


struct configprivate
{
    QDomDocument doc;   // xml文件对象
    xml xp;             // xml对象
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
    p->xp.setFilePath(filename);
    p->doc = p->xp.getDocument();
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
    p->xp.WriteToXML();

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
    WriteToXML("");
    this->close();
}
void NetworkConfig::reject()
{
    this->close();
}
