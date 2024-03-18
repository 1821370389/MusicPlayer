#ifndef NETWORKCONFIG_H
#define NETWORKCONFIG_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class NetworkConfig;
}
QT_END_NAMESPACE

struct configprivate;

class NetworkConfig : public QWidget
{
    Q_OBJECT

public:
    NetworkConfig(QWidget *parent = nullptr);
    ~NetworkConfig();
    bool ReadFromXML(const QString &filename);
    bool WriteToXML(const QString &filename);
    std::pair<QString,int> GetNetWorkConfig();
public slots:
    void accept();
    void reject();

private:
    Ui::NetworkConfig *ui;
    configprivate* p;
};
#endif // NETWORKCONFIG_H
