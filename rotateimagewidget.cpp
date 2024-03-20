#include "rotateimagewidget.h"
#include <Qtimer>
#include <QPainter>
#include <QPainterPath>
#include <QTransform>
#include <iostream>

struct RotateImageWidgetPrivate
{
    int rotateDegree;  // 旋转角度
    QTimer timer;       // 更新的定时器
    QPixmap picture;    // 旋转图片
    int r;              // 圆的半径
    QPoint center; 
};
// 无图构造
RotateImageWidget::RotateImageWidget(QWidget *parent)
    : QWidget(parent),p(new RotateImageWidgetPrivate)
{
    connect(&p->timer, &QTimer::timeout, this, &RotateImageWidget::UpdateScene);
    p->timer.setInterval(50);
}
// 带图构造
RotateImageWidget::RotateImageWidget(const QPixmap &picture, QWidget *parent)
    :QWidget(parent),p(new RotateImageWidgetPrivate)
{
    connect(&p->timer, &QTimer::timeout, this, &RotateImageWidget::UpdateScene);
    p->timer.setInterval(50);
    setPixmap(picture);
}
// 设置图片
void RotateImageWidget::setPixmap(const QPixmap &picture)
{
    p->picture = picture;
    p->rotateDegree = 0;
    p->r = std::min(this->width(), this->height()) / 2;
    p->center = QPoint(this->width() / 2,this->height() / 2);
}
// 开始旋转计时
void RotateImageWidget::startTimer()
{
    p->timer.start();
}
// 暂停旋转计时
void RotateImageWidget::stopTimer()
{
    p->timer.stop();
}
// 析构
RotateImageWidget::~RotateImageWidget()
{
    delete p;
}
// 更新旋转角度
void RotateImageWidget::UpdateScene()
{
    p->rotateDegree = (p->rotateDegree + 2) % 360;
    update();
}
// 窗口绘制事件
void RotateImageWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPoint(p->center);
    painter.setRenderHint(QPainter::Antialiasing);  // 抗锯齿

    // 裁剪路径
    QPainterPath painterPath;
    painterPath.addEllipse(p->center,p->r,p->r);    // 裁剪一个(椭)圆

    // 旋转
    QTransform transform;
    transform.translate(p->center.x(),p->center.y());
    transform.rotate(p->rotateDegree);

    painter.setClipPath(painterPath);
    painter.setTransform(transform, true);

    painter.drawPixmap(-p->r, -p->r, p->r*2, p->r*2, p->picture);
}
// 当窗口大小变动时，重新计算圆心及圆的半径
void RotateImageWidget::resizeEvent(QResizeEvent *event)
{
    p->r = std::min(this->width(), this->height()) / 2;
    p->center = QPoint(this->width() / 2,this->height() / 2);
}
