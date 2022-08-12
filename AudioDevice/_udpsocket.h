#ifndef _UDPSOCKET_H
#define _UDPSOCKET_H

#include <QObject>
#include <qudpsocket.h>

class _UdpSocket: public QObject
{
public:
    explicit _UdpSocket(QObject *parent = nullptr);

    bool startUdp(QString ip, int port);

    void stopUdp();

    QUdpSocket *getUdpSocket();
private:
    QUdpSocket *udpSocket;//用于音频信号的发送
};
extern _UdpSocket *myUdpSocket;

#endif // _UDPSOCKET_H
