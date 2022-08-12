#include "inputsound.h"
#include <QMessageBox>
#include "_udpsocket.h"

InputSound::InputSound(QObject *parent,QString IPs,int Ports) : QObject(parent)
    ,IP(IPs),Port(Ports)
{
    aData.bIndex=aData.fIndex=0;
    memset(aData.data,0,READ_MAX_SIZE);
    destaddr.setAddress(IP);

    //udpSocket= new QUdpSocket(this);
    //绑定  端口，不绑定也行，因为是发送端不涉及到接收
//    myUdpSocket->getUdpSocket()->bind(destaddr,Port);

    qDebug()<<"InputSound:"<<destaddr.toString()<<Port;
    QAudioFormat format;
        format.setSampleRate(8000);//设置采样率 每秒钟取得声音样本的次数
        format.setChannelCount(1);//设定声道数目，mono(平声道)的声道数目是1；stero(立体声)的声道数目是2
        format.setSampleSize(16);//设置采样大小，一般为8位或16位
        format.setCodec("audio/pcm");//编码器
        format.setSampleType(QAudioFormat::SignedInt);//设置样本数据类型
        format.setByteOrder(QAudioFormat::LittleEndian);//设定高低位,LittleEndian（低位优先）/LargeEndian(高位优先)
    Audioinput = new QAudioInput(format,this);
    Audioinput->setVolume(1);
}
void strcpyByIndex2(char*aa,char*bb,int len)
{
    while(len--){
        *aa=*bb;
        aa++;bb++;
    }
}

void InputSound::sendMic_Data()
{
        if((aData.bIndex+512)>READ_MAX_SIZE)
        {
            aData.bIndex=0; //qDebug()<<"~~~~~~~~>READ_MAX_SIZE"<<aData.bIndex+512;
        }
        if((aData.bIndex+512)>aData.fIndex && abs(aData.bIndex+512-aData.fIndex)<1000)
        {
            qDebug()<<"sendMic_Data return";
            return;         //qDebug()<<"~~~~~~>=aData.fIndex"<<aData.bIndex+512;
        }
        memset(temp_data,0,522);
        char astr[11]="mic_data\r\n";
        strcpyByIndex2(temp_data,astr,10);
        strcpyByIndex2(temp_data+10,aData.data+aData.bIndex,512);
        myUdpSocket->getUdpSocket()->writeDatagram((const char *)temp_data,522,destaddr,Port);
//        qDebug()<<"对讲1"<<destaddr<<Port;
        aData.bIndex+=512;
}

void InputSound::sendMic_Data(char *datas)
{
    memset(temp_data,0,522);
    char astr[11]="mic_data\r\n";
    strcpyByIndex2(temp_data,astr,10);
    strcpyByIndex2(temp_data,datas,512);
    myUdpSocket->getUdpSocket()->writeDatagram(temp_data,522,destaddr,Port);
    qDebug()<<"对讲2"<<destaddr<<Port<<temp_data;
}

void InputSound::onReadyRead(){
    char tData[640];
    memset(tData,0,640);
    qint64 len = device->read(tData,640);//读取音频
    if(len==640)
    {
        strcpyByIndex2(aData.data+aData.fIndex,tData,640);
        aData.fIndex+=640;
        if(aData.fIndex>=READ_MAX_SIZE)
        {
            aData.fIndex=0;
        }
        sendMic_Data();
        sendMic_Data();
    }
}

//广播开始
void InputSound::StartAudio(int ports)
{
    Port=ports;
    device=Audioinput->start();
    connect(device,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    if(!device)
        QMessageBox::information(NULL,"micError"," mic麦克风设备异常.");
}

//广播开始2
void InputSound::StartAudio(QString ip,int ports)
{
    Port=ports;
    if(ip=="")
    {
    //全局广播
    //  destaddr.setAddress(QHostAddress::Broadcast);
        qDebug()<<"talking1 StartAudio...."<<"10.0.255.255"<<ports;
        destaddr.setAddress("10.0.255.255");
    }else
    {
        qDebug()<<"talking2 StartAudio...."<<ip<<ports;
        destaddr.setAddress(ip);
    }
    qDebug()<<"talking3 StartAudio...."<<ip<<ports;
    device=Audioinput->start();
    connect(device,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    if(!device)
        QMessageBox::information(NULL,"micError","麦克风设备异常.");
}

//停止
void InputSound::StopAudio()
{
    Audioinput->stop();
//    device->close();
//    device->deleteLater();
}
