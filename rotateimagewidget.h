#ifndef ROTATEIMAGEWIDGET_H
#define ROTATEIMAGEWIDGET_H
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class RotateImageWidget;
}
QT_END_NAMESPACE

struct RotateImageWidgetPrivate;

class RotateImageWidget : public QWidget
{
    Q_OBJECT

public:
    RotateImageWidget(QWidget *parent = nullptr);
    RotateImageWidget(const QPixmap &picture, QWidget *parent = nullptr);
    ~RotateImageWidget();
    void setPixmap(const QPixmap &picture);
    void startTimer();
    void stopTimer();
public slots:
    void UpdateScene();

protected:
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    RotateImageWidgetPrivate* p;
};
#endif // ROTATEIMAGEWIDGET_H
