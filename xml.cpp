#include "xml.h"

#include <QFile>
#include <QDebug>


struct xmlArgs
{
    QString filePath;
    QDomDocument doc;
};

xml::xml(): args(new xmlArgs) 
{

}
xml::xml(const QString& filePath): args(new xmlArgs)
{
    setFilePath(filePath);
}
xml::~xml()
{
    delete args;
}
void xml::setFilePath(const QString& filePath)
{
    args->filePath = filePath;
    ReadFromXML();
}

QDomDocument &xml::getDocument()
{
    return args->doc;
}

bool xml::ReadFromXML()
{
    QFile file(args->filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error opening file";
        return false;
    }
    QString errorStr;
    int row, col;
    if (!args->doc.setContent(&file, &errorStr, &row, &col))
    {
        qDebug() << "Error parsing XML file\n Path:" << args->filePath << "\n Error:" << errorStr;
        qDebug() << "Line:" << row << "Col:" << col;
        file.close();
        return false;
    }
    qDebug() << "File parsed successfully";
    file.close();
    return true;
}

bool xml::WriteToXML()
{
    QFile file(args->filePath);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Error opening file";
        return false;
    }
    if (file.write(args->doc.toByteArray()) <= 0)
    {
        qDebug() << "Error writing to file";
        file.close();
        return false;
    }
    file.close();
    return true;
}

// QDomElement &xml::FindByValue(const QString &value)
// {
//     // return args->doc.documentElement().firstChildElement(value);
//     return new QDomElement;
// }
