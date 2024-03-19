#include "mainwindow.h"
#include "login.h"
#include "rotateimagewidget.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* // 界面翻译
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "MusicPlayer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    */
    /**/
    // MainWindow w;
    // w.show();
    Login l;
    l.show();

    QFile qssfile(":/style.qss");
    if(qssfile.open(QIODevice::ReadOnly))
    {
        // Qt应用指针
        qApp->setStyleSheet(qssfile.readAll());
        qssfile.close();
    }

    // RotateImageWidget w;
    // w.setPixmap(QPixmap(":/5.png"));
    // w.show();

    // 时间循环：死循环
    return a.exec();
}
