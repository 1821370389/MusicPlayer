#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct MusicPlayer;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();    
    void SetLRC(const QString& path);
    void LoadListFromDir(const QString& path);     // 从本地文件中加载歌单
    void makeRandList(const int& size);
public slots:
    void PlayMusic();
    void SetDurationTime();
    void SetPositionTime();
    void ChangeLRC();
    void ChangeVolume();
    void ItemDoubleClicked();
    void PreMusic();
    void NextMusic();
    void ChangeMode();
    void SetPlayState();

    void NetWorkModify();
    void networkConnect();
    void socketConnected();
private:
    Ui::MainWindow *ui;
    MusicPlayer *mp;
};
#endif // MAINWINDOW_H
