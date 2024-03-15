#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QTimer>
#include <QDir>
#include <QList>


enum MusicMode
{
    LIST_MODE = 0,
    SINGLE_MODE,
    RANDOM_MODE
};

QList<int> randLsit;
int randIndex = 0;


struct MusicPlayer
{
    QAudioOutput* audioOutput;
    QMediaPlayer* player;   // 播放器对象
    // std::map<int,QString> LRCMap; // 歌词列表
    std::multimap<int,QString> LRCMap;  // 双语列表
    QTimer timer; // 歌词定时器
    QString DirPath;    // 路径
    int mode;           // 播放模式
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), mp(new MusicPlayer)
{
    ui->setupUi(this);
    mp->DirPath = "D:\\ProgramData\\VScode\\C++\\QT\\MusicPlayer\\music";
    mp->mode = 0;

    this->setWindowTitle("音乐播放器");
    this->setWindowIcon(QIcon(":/3.png"));

    // QPixmap background(":/5.png");
    // background.scaled(this->size(), Qt::IgnoreAspectRatio);
    // QPalette palette;
    // palette.setBrush(backgroundRole(), background);
    // this->setPalette(palette);

    // 设置图片及修改大小
    // QPixmap post =  QPixmap(":/2.png");
    // ui->label->setPixmap(post.scaled(ui->label->width(),ui->label->height()));


    mp->player = new QMediaPlayer;
    mp->audioOutput = new QAudioOutput;
    mp->player->setAudioOutput(mp->audioOutput);
    // mp->audioOutput->setVolume(50);
    // 设置多媒体路径
    // player->play();


    ui->lrc_label->setText("");
    ui->lrc_label_2->setText("");
    ui->volumeLabel->setText("100");
    ui->PrevButton->setIcon(QIcon(":/Prev.png"));
    ui->PlayButton->setIcon(QIcon(":/Play.png"));
    ui->NextButton->setIcon(QIcon(":/Next.png"));
    ui->modeButton->setText("列表循环");
    ui->verticalSlider->setMaximum(100);
    ui->verticalSlider->setMinimum(0);
    ui->verticalSlider->setValue(100);

    LoadListFromDir(mp->DirPath);

    connect(ui->PlayButton, &QPushButton::clicked, this, &MainWindow::PlayMusic);
    connect(ui->PrevButton, &QPushButton::clicked, this, &MainWindow::PreMusic);
    connect(ui->NextButton, &QPushButton::clicked, this, &MainWindow::NextMusic);
    connect(ui->modeButton, &QPushButton::clicked, this, &MainWindow::ChangeMode);
    connect(mp->player, &QMediaPlayer::durationChanged, this, &MainWindow::SetDurationTime);
    connect(mp->player, &QMediaPlayer::positionChanged, this, &MainWindow::SetPositionTime);
    connect(mp->player, &QMediaPlayer::playbackStateChanged, this, &MainWindow::SetPlayState);
    connect(ui->horizontalSlider, &QSlider::sliderMoved, mp->player, &QMediaPlayer::setPosition);
    connect(ui->verticalSlider, &QSlider::sliderMoved, this, &MainWindow::ChangeVolume);
    connect(&mp->timer, &QTimer::timeout, this, &MainWindow::ChangeLRC);
    connect(ui->listWidget, &QListWidget::doubleClicked, this, &MainWindow::ItemDoubleClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete mp;
}
void MainWindow::PlayMusic()
{
    if(ui->listWidget->currentRow() == -1)
    {
        ui->listWidget->setCurrentRow(0);
        ItemDoubleClicked();
        return;
    }
    if(mp->player->isPlaying())
    {
        mp->player->pause();
        ui->PlayButton->setIcon(QIcon(":/Play.png"));
        qDebug() << "pause";
        mp->timer.stop();
    }
    else
    {
        mp->player->play();
        ui->PlayButton->setIcon(QIcon(":/Pause.png"));
        qDebug() << "play";
        mp->timer.start(500);
    }
}

void MainWindow::SetDurationTime()
{
    int minute = mp->player->duration() / 1000 /60;
    int second = mp->player->duration() / 1000 % 60;
    QString str = QString("%1:%2").arg(minute).arg(second);
    ui->total_duration_label->setText(str);
    ui->horizontalSlider->setMaximum(mp->player->duration());
}

void MainWindow::SetPositionTime()
{
    int minute = mp->player->position() / 1000 /60;
    int second = mp->player->position() / 1000 % 60;
    QString str = QString("%1:%2").arg(minute).arg(second);
    ui->curent_time_lable->setText(str);
    ui->horizontalSlider->setValue(mp->player->position());
}

qint64 SetLRCTime(QString& time)
{
    time.remove(0,1);
    QStringList cutTime = time.split(":");
    int min = cutTime[0].toInt();
    int s = cutTime[1].split(".")[0].toInt();
    int ms = cutTime[1].split(".")[1].toInt();
    return min * 1000 * 60 + s * 1000 + ms;
}

void MainWindow::SetLRC(const QString& path)
{
    mp->LRCMap.clear();
    ui->lrc_label_2->setText("");
    ui->lrc_label->setText("");
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件打开失败！\n";
        return;
    }
    // 文本流
    QTextStream text(&file);
    while(!text.atEnd())
    {
        QString line = text.readLine();
        QStringList list = line.split("]");
        if(list.size() < 2)
        {
            continue;
        }
        QString lyric = list[1];
        qint64 time = SetLRCTime(list[0]);
        mp->LRCMap.insert({time, lyric});
    }
    file.close();
}

void MainWindow::LoadListFromDir(const QString& path)
{
    QDir dir(path);
    if(!dir.exists())
    {
        qDebug() << "文件夹路径不存在\n" ;
    }
    #if 1
    QFileInfoList list = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    for(auto it = list.begin(); it != list.end(); it++)
    {
        if(it->suffix() == "mp3")
        {
            // QString name = it->baseName();
            // ui->listWidget->addItem(name);
            // continue;
            QListWidgetItem* item = new QListWidgetItem(it->baseName());
            item->setData(Qt::UserRole, path);
            ui->listWidget->addItem(item);
        }
        if(it->isDir())
        {
            LoadListFromDir(it->absoluteFilePath());
        }
        
    }
    #else
    QStringList list = dir.entryList(QDir::Files);
    for(auto item : list)
    {
        qDebug() << item << "\n";
    }
    #endif
}

void MainWindow::ChangeLRC()
{
    if(mp->LRCMap.empty())
    {
        return;
    }
    qint64 currentTime = mp->player->position();
    for(auto it = mp->LRCMap.begin(); it != mp->LRCMap.end();)
    {
        auto it_prev = it;
        bool flag = false;
        it++;
        if(it != mp->LRCMap.end())
        {
            if(it->first == it_prev->first)
            {
                flag = true;
                ui->lrc_label->setText("");
                it++;
            }
            if(it->first >= currentTime && it_prev->first <= currentTime)
            {
                ui->lrc_label_2->setText(it_prev->second);
                it_prev++;
                if(flag)
                {
                    ui->lrc_label->setText(it_prev->second);
                    it_prev++;
                    flag = false;
                }
                return;
            }
        }
        else
        {
            ui->lrc_label->setText(it_prev->second);
            return;
        }
    }
}

void MainWindow::ChangeVolume()
{
    float volume = ui->verticalSlider->value();
    ui->volumeLabel->setText(QString::number(volume));
    mp->audioOutput->setVolume(volume / 100.0);
    qDebug() << volume;
}

QString GetDirPost(const QString& dirPath)
{
    QDir dir(dirPath);
    if(!dir.exists())
    {
        return "";
    }
    QFileInfoList list = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    for(auto it = list.begin(); it != list.end(); it++)
    {
        if(it->suffix() == "jpg" || it->suffix() == "png")
        {
            return it->absoluteFilePath();
        }
    }
    return "";
}

void MainWindow::ItemDoubleClicked()
{
    QString musicName = ui->listWidget->currentItem()->text();
    QString dirPath = ui->listWidget->currentItem()->data(Qt::UserRole).toString();
    mp->player->setSource(QUrl::fromLocalFile(dirPath + "\\" + musicName + ".mp3"));
    QString LRCpath = dirPath + "\\" + musicName + ".lrc";
    SetLRC(LRCpath);

    QString postPath = GetDirPost(dirPath);
    if(postPath.isEmpty())
    {
        ui->label->setPixmap(QPixmap(":/2.png"));
    }
    else
    {
        ui->label->setPixmap(QPixmap(postPath).scaled(ui->label->width(),ui->label->width()));
    }

    PlayMusic();
}

void MainWindow::makeRandList(const int& size)
{
    srand(time(NULL));
    randLsit.clear();
    QList<int> list;
    for(int i = 0; i < size; i++)
    {
        list.append(i);
    }
        for(int i = 0; i < size; i++)
    {
        int index = rand() % list.size();
        randLsit.append(list[index]);
        list.removeAt(index);
    }

}

void MainWindow::PreMusic()
{   int index;
    if(mp->mode == RANDOM_MODE)
    {
        if(randIndex == 0)
        {
            makeRandList(ui->listWidget->count());
        }
        randIndex = randIndex - 1 < 0 ? ui->listWidget->count() - 1 : randIndex - 1;
        index = randLsit[randIndex % ui->listWidget->count()];
    }
    else
    {
        randIndex = 0;
        index = ui->listWidget->currentRow();
        index = index - 1 < 0 ? ui->listWidget->count() - 1 : index - 1;
    }
    ui->listWidget->setCurrentRow(index);
    ItemDoubleClicked();
}

void MainWindow::NextMusic()
{   int index;
    if(mp->mode == RANDOM_MODE)
    {
        if(randIndex == 0)
        {
            makeRandList(ui->listWidget->count());
        }
        
        index = randLsit[++randIndex % ui->listWidget->count()];
    }
    else
    {
        randIndex = 0;
        index = ui->listWidget->currentRow();
        index = index + 1 >= ui->listWidget->count() ? 0 : index + 1;
    }
    ui->listWidget->setCurrentRow(index);
    ItemDoubleClicked();
}

void MainWindow::ChangeMode()
{
    switch (mp->mode)
    {
    case LIST_MODE:
        ui->modeButton->setText("单曲循环");
        mp->mode = SINGLE_MODE;
        break;

    case SINGLE_MODE:
        ui->modeButton->setText("随机播放");
        mp->mode = RANDOM_MODE;
        break;

    case RANDOM_MODE:
        ui->modeButton->setText("列表循环");
        mp->mode = LIST_MODE;
    default:
        break;
    }
}

void MainWindow::SetPlayState()
{
    /* 下一曲 */
    if(mp->player->playbackState() == QMediaPlayer::StoppedState && mp->player->position() >= mp->player->duration())
    {
        if(mp->mode == SINGLE_MODE)
        {
            // mp->player->setPosition(0);
            mp->player->play();
            // PlayMusic();
            // PlayMusic();
        }
        else
        {
            NextMusic();
        }
    }
}
