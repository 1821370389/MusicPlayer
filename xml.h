#ifndef XML_H
#define XML_H
#include <QString>
#include <QDomDocument>

struct xmlArgs;

class xml
{
public:
    xml();
    xml(const QString& filePath);
    ~xml();
    void setFilePath(const QString& filePath);
    QDomDocument& getDocument();
    bool ReadFromXML();
    bool WriteToXML();
    // QDomElement& FindByValue(const QString& value);

private:
    xmlArgs* args;
};

#endif // XML_H
