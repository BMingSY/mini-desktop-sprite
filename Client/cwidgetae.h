#ifndef CWIDGETAE_H
#define CWIDGETAE_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

class CWidgetAE : public QWidget
{
    Q_OBJECT
public:
    explicit CWidgetAE(QString strImgDir,QWidget *parent = nullptr);

    void setImgDir(QString strImgDir); //传入图片文件夹路径  就会自动开始播放 50ms刷一帧
    void closeAE(); //关闭动画显示  停止刷新

    void showWide() ;
private:
    void startAE();  // 设置循环时间
    void stopAE(); // 停止循环
protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
private:
    QTimer *m_pTimer = nullptr;
    QString m_strImgDir = "";
    QStringList m_strListImg;
    int m_nIndex = 0;
    int m_nTimerId = 0;
};

#endif // CWIDGETAE_H
