#ifndef INPUTSOUND_H
#define INPUTSOUND_H

#include <QObject>
#include <QWidget>
#include <QtNetwork/QUdpSocket>
#include <QAudio>//一下这五个是QT处理音频的库
#include <QAudioFormat>
#include <QAudioOutput>
#include <QIODevice>
#include <QHostAddress>
#include <QDebug>
#include <QtMultimedia/QAudioInput>

#define READ_MAX_SIZE 25600
class InputSound : public QObject
{
    Q_OBJECT
public:
    explicit InputSound(QObject *parent = nullptr,QString IP="10.0.255.255",int Port=5004);

public:
//        QUdpSocket *udpSocket;//用于音频信号的发送
        QAudioInput *Audioinput;
        QIODevice   *device;
        char temp_data[522];
        struct audioData{
            char data[READ_MAX_SIZE];
            int  fIndex;//front在前的缓存指针地址。
            int  bIndex;//back在后的缓存指针地址。
        };

        QHostAddress destaddr;//QHostAddress::Any
        audioData aData;
        void sendMic_Data();
        void StopAudio();
        void StartAudio(int ports);
        void StartAudio(QString ip, int ports);
        void sendMic_Data(char *datas);
private:
        QByteArray byteArrayTEMp;
        QString IP;
        int Port;
private slots:
        void onReadyRead();//
};

#endif // INPUTSOUND_H
