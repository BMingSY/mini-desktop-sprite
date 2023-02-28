#include "cwidgetae.h"
#include <QDir>
#include <QDebug>

CWidgetAE::CWidgetAE(QString strImgDir, QWidget *parent)
    : QWidget(parent)
{
    setImgDir(strImgDir);
}

void CWidgetAE::setImgDir(QString strImgDir){
    m_strImgDir = strImgDir;  // 设置图片的路径

    QDir tmpDir(m_strImgDir);
    QFileInfoList listInfo = tmpDir.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot,QDir::Name);

    m_strListImg.clear();
    // 将所有的图片路径加入一个String list之中
    foreach(QFileInfo strFileInfo,listInfo)
        m_strListImg.append(strFileInfo.filePath());


    this->setWindowFlag(Qt::FramelessWindowHint) ;
    this->setAttribute(Qt::WA_TranslucentBackground) ;  // 设置背景透明等等
    this->resize(QImage(m_strListImg.first()).size());   // 重新设置大小到图片的大小                                        // 开始进行展示

}

void CWidgetAE::closeAE()
{
    m_strImgDir = "";
    stopAE();
    this->hide();
}

void CWidgetAE::showWide()
{
    stopAE();
    this->show() ;
    startAE();
}

void CWidgetAE::startAE(){
    m_nTimerId = startTimer(200);
}

void CWidgetAE::stopAE()
{
    if(0 == m_nTimerId)
        return ;

    killTimer(m_nTimerId);
    m_nTimerId = 0;
}

void CWidgetAE::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    if(m_nIndex > (m_strListImg.size() - 1))
        return ;

    QPainter painter(this);
    painter.drawImage(QPointF(0,0),QImage(m_strListImg[m_nIndex]));

    m_nIndex++;
    m_nIndex = (m_nIndex)%m_strListImg.size();
}

void CWidgetAE::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    this->update();
}

