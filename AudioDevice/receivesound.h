#ifndef RECEIVESOUND_H
#define RECEIVESOUND_H
#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QAudio>
#include <QAudioInput>
#include <QAudioFormat>
#include <QIODevice>
#include <QtMultimedia/QAudioOutput>
#define MAX_SIZE_DATA 10440

class ReceiveSound : public QObject
{
    Q_OBJECT
public:
    explicit ReceiveSound(QObject *parent = nullptr);

    QAudioOutput *Audiooutput;
    QIODevice *device;
    void searchMic_data();
    void StopUdp();
    void StartUdp();

    void PlayData(const char *data, qint64 len);
    void PlayData(const char *data);
    void searchMic_data2(const char *datas, int js=0);
private:
//    QUdpSocket *udpsocket;
    struct UdpData{
        char udpData[MAX_SIZE_DATA];
        int  fIndex;//储存Index;  front在前的缓存指针地址。
        int  bIndex;//播放写入数据的index  back在后的缓存指针地址。
    };
    quint32 localIP;
    UdpData data;
    int timeID;
   struct video{
         char data[1024];
         int lens;
     };
     QByteArray byteArray;
 private slots:
     void readyReadSlot();
 signals:
     void shootIp(QString ip);
};

#endif // RECEIVESOUND_H
