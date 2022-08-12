#ifndef UPDNOTICE_H
#define UPDNOTICE_H

#include <QMessageBox>
#include <QObject>
#include <QThread>
#include <QtNetwork/qudpsocket.h>
//udp分线程接受 BT+SOS信息
class UdpNotice : public QObject
{
    Q_OBJECT
public:
    explicit UdpNotice(int ports=7686 , QObject *parent = nullptr);
//    QUdpSocket *udpsocket;
    void stopUdp();
    int port;
    QThread *thread;
    QStringList lStr;//保存信息
    int id;
    QTimer *timer;
signals:
    void shootMessages(QString);
public slots:
    void startUdpSos();
    void readyReadSlot();
    void buttonCLicked(QAbstractButton *button);
    void update();
};

#endif // UPDNOTICE_H
