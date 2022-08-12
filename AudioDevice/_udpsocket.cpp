#include "_udpsocket.h"
#include <QMessageBox>
#include <QUdpSocket>
#include <QWidget>

_UdpSocket *myUdpSocket=new _UdpSocket();

_UdpSocket::_UdpSocket(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
}

QUdpSocket *_UdpSocket::getUdpSocket()
{
        if(udpSocket==NULL)
        {
            udpSocket = new QUdpSocket();
        }
        return udpSocket;
}

//开始监听 false断开 true连接
bool _UdpSocket::startUdp(QString ip, int iPort)
{
    //如果绑定,解除绑定
    if(QAbstractSocket::BoundState==udpSocket->state())
    {
        stopUdp();
    }
    //如果未绑定进行绑定
    if(QAbstractSocket::UnconnectedState==udpSocket->state())
    {
        if(ip=="any")
        {
            udpSocket->bind(QHostAddress::Any,iPort);

            QString pinStr=QString("【IP:%1 Port:%2 开始监听】-")
                    .arg("any")
                    .arg(iPort);
            qDebug()<<pinStr;
        }else
        {
            QHostAddress *testaddr=new QHostAddress;

            testaddr->setAddress(ip);

            udpSocket->bind(*testaddr,iPort);

            qDebug()<<"local IP:"<<testaddr<<iPort;

            QString pinStr=QString("【IP:%1 Port:%2 开始监听】-")
                    .arg(testaddr->toString())
                    .arg(iPort);
            qDebug()<<pinStr;
        }
    }

    //绑定失败，关闭udpsocket
    if(QAbstractSocket::UnconnectedState==udpSocket->state())
    {
        QString str=QString("无法连接 %1:%2\r\nudpsocket state:%3\r\nUdpSocket已关闭。")
                .arg(ip)
                .arg(iPort)
                .arg(udpSocket->state());
        qDebug()<<str;
        stopUdp();
        QMessageBox::warning(NULL,"UDP连接失败",str);
        return false;
    }
    return true;
}

//停止udp广播
void _UdpSocket::stopUdp()
{
    udpSocket->close();
     qDebug()<<QString("【UDP断开状态(0关闭4绑定):%1】").arg(udpSocket->state());
}
