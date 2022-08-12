#include "receivesound.h"

#include <QMessageBox>
#include <QNetworkInterface>
#include <mainwidget.h>
#include "_udpsocket.h"

//读取网卡列表 ；MAC  获取广播地址  例如：192.168.9.255
quint32 getIpAddresst()
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    QString newMAC = "";
    int Address = 0;
    QString Netmask = "";
    QString Broadcast = "";
    foreach(QNetworkInterface interface,list)
    {
//      qDebug() << "Device:"<<interface.name();
//      qDebug() << "mac:"<<interface.hardwareAddress();
        newMAC = interface.hardwareAddress();
        //取 IP 地址
        QList<QNetworkAddressEntry> entryList = interface.addressEntries();
        foreach( QNetworkAddressEntry entry,entryList)
        {
            QHostAddress address2 = entry.ip();
            if( address2.protocol() == QAbstractSocket::IPv4Protocol )
            {
//            qDebug()<< "IPV4 Address:" <<entry.ip().toString();
//            qDebug()<< "Netmask: " <<entry.netmask().toString();
//            qDebug()<< "Broadcast:" <<entry.broadcast().toString();
              Address = entry.ip().toIPv4Address();
              Netmask = entry.netmask().toString();
              Broadcast = entry.broadcast().toString();
              if(entry.ip().toString()!="127.0.0.1")
                return Address;
            }
        }
    }
    return Address;
}

ReceiveSound::ReceiveSound(QObject *parent) : QObject(parent)
{
    data.fIndex=data.bIndex=0;

    memset(data.udpData,0,MAX_SIZE_DATA);
//    udpsocket = new QUdpSocket(this);    //  udpsocket->bind(QHostAddress::Any,5004);
    connect(myUdpSocket->getUdpSocket(),SIGNAL(readyRead()),this,SLOT(readyReadSlot()));//收到网络数据报就开始往outputDevice写入，进行播放

    QAudioFormat format;
    format.setSampleRate(8000);//赫兹
    format.setChannelCount(1);
    format.setSampleSize(16);//将样本大小设置为指定的采样大小(以比特为单位)。
    format.setCodec("audio/pcm");//这个函数的参数应该是QAudioDeviceInfo::supportedCodecs()
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);

    Audiooutput = new QAudioOutput(format,this);
    device = Audiooutput->start();//开始播放
    Audiooutput->setVolume(1);
    localIP=getIpAddresst();
    qDebug()<<localIP<<":locaip";
}
void ReceiveSound::PlayData(const char*data,qint64 len)
{
    device->write(data,len);
}
void ReceiveSound::PlayData(const char*data)
{
    device->write(data);
}

void ReceiveSound::StopUdp()
{
    Audiooutput->stop();
    myUdpSocket->stopUdp();
}
void ReceiveSound::StartUdp()
{
    device=Audiooutput->start();
    if(!device)
       QMessageBox::information(NULL,"micError","播放设备异常.");
     myUdpSocket->startUdp(mainWidget::UdpSocketIP,mainWidget::UdpSocketPort);
}

void strcpyByIndex(char*aa,char*bb,int len)
{
    if(len<0)return;
    while(len--)
    {
        *aa=*bb;
        aa++;bb++;
    }
}

void ReceiveSound::readyReadSlot()//收到数据
{
    while(myUdpSocket->getUdpSocket()->hasPendingDatagrams())
    {
        qint64 size=myUdpSocket->getUdpSocket()->pendingDatagramSize();
        qDebug()<<"收到数据post size"<<size;
        if(size==522)
        {
            QHostAddress senderip;
            quint16 senderport;
            char tdata[522];
            memset(tdata,0,522);
            myUdpSocket->getUdpSocket()->readDatagram(tdata,522,&senderip,&senderport);
//          qDebug()<<"收到数据："<<senderip.toString()<<senderport<<tdata;
            qDebug()<<"收到数据ip address："<<senderip.toIPv4Address()<<senderip.toString().mid(7)<<tdata;
            QString ipAddress=senderip.toString().mid(7);//::ffff:10.0.50.255
            emit shootIp(ipAddress);
            if(senderip.toIPv4Address() == localIP)
            {
//              qDebug()<<"回流数据";
                return; //回流数据
            }
//          if(data.fIndex>=MAX_SIZE_DATA)
//              data.fIndex=0;
//          strcpyByIndex(data.udpData+data.fIndex , tdata, 522);
//          data.fIndex+=522;
            searchMic_data2(tdata);
            qDebug()<<"post size mic1111";
        }else if(size==1044)
        {
            QHostAddress senderip;
            quint16 senderport;
            char tdata[1044];
            memset(tdata,0,1044);
            myUdpSocket->getUdpSocket()->readDatagram(tdata,1044,&senderip,&senderport);

            if(senderip.toIPv4Address() == localIP )
            {
//                qDebug()<<"回流数据";
                return;
            }
            searchMic_data2(tdata,1);
            qDebug()<<"post size mic1122";
            searchMic_data2(tdata+512,2);
            qDebug()<<"post size mic1133";
        }
        else//非522读取，防止死循环
        {
            QHostAddress senderip;
            quint16 senderport;
//          qDebug()<<"udpsocket!=522"<<udpsocket->pendingDatagramSize()<<endl;
            char tdata[size];
            memset(tdata,0,size);
            myUdpSocket->getUdpSocket()->readDatagram(tdata,size,&senderip,&senderport);
//          searchMic_data2(tdata);
            return;
        }
    }
}

void ReceiveSound::searchMic_data2(const char *datas,int js)//  mic_data\r\n
{
    for(int i=0;i<522;i++)
    {
        if(datas[i]=='m'&&
            datas[i+1]=='i'&&
            datas[i+2]=='c')
        {
            device->write(datas+i+10,512);
            qDebug()<<"data2"<<js;
            return;
        }
    }
}

void ReceiveSound::searchMic_data()//  mic_data\r\n
{
    int ss=0;
    for(int i=data.bIndex;i<=MAX_SIZE_DATA;i++)
    {
        if(data.udpData[i]=='m'&&
      data.udpData[i+1]=='i'&&
      data.udpData[i+2]=='c'
    //data.udpData[(i+8)>=MAX_SIZE_DATA?(i+8-MAX_SIZE_DATA):(i+8)]=='\r'&&
    //data.udpData[(i+9)>=MAX_SIZE_DATA?(i+9-MAX_SIZE_DATA):(i+9)]=='\n'
       )
        {
            data.udpData[(i+9)>=MAX_SIZE_DATA?(i+9-MAX_SIZE_DATA):(i+9)] = 0;//防止第二次读取
            if((i+10)>=MAX_SIZE_DATA)
            {
                i=i+10-MAX_SIZE_DATA;
            }else
            {
                i=i+10;
            }
            qDebug()<<"write"<<ss++ <<i;
            if(i>=data.fIndex&& abs(i-data.fIndex)<1044)
            {
                qDebug()<<">>>>>>>>>>>>>>>>> write.index=:"<<i;
                qDebug()<<"return";
                return;
            }
            if((i+512)<MAX_SIZE_DATA)
            {
                data.bIndex=i+512;
                device->write(data.udpData+i,512);
                return;
            }
        }
        if(i>=MAX_SIZE_DATA)//防止卡索引到最后而搜索不到mic_data
        {
            data.bIndex=0;
        }
    }
}
bool isDot_data(char *txdata) //判读是否打点数据 doc_data\r\n
{
    if(*txdata==NULL)
    {
        qDebug()<<"空数据...";
        return true;
    }
    if(  txdata[0]=='d'&&
    //         txdata[1]=='o'&&
    //         txdata[2]=='c'&&
    //         txdata[3]=='_'&&
    //         txdata[4]=='d'&&
    //         txdata[5]=='a'&&
    //         txdata[6]=='t'&&
    //         txdata[7]=='a'&&
    //         txdata[8]=='\r'&&
         txdata[9]=='\n')
    {
        qDebug()<<"打点数据返回不处理...";
        return true;
    }
    return false;
}
bool deleteCharFront(char *txdata,int len)//删除mic_data\r\n 或 mic_data\rh
{
    if(*txdata==NULL||len<0)
        return false;
    if(  txdata[0]=='m'&&
         txdata[1]=='i'&&
         txdata[2]=='c'&&
         txdata[3]=='_'&&
         txdata[4]=='d'&&
         txdata[5]=='a'&&
         txdata[6]=='t'&&
         txdata[7]=='a'&&
         txdata[8]=='\r')//if(txdata[9]=='\n'数据    or  =='h'心跳
    {
            qDebug()<<"len:"<<len;
            while(len!=0)
            {
                *txdata = *(txdata+10);
                txdata++;
                len--;
            }
    }
    return false;
}
bool deleteCharFrontPaste(char *txdata,int len)//覆盖mic_data\r\n 或 mic_data\rh 为0
{
    if(*txdata==NULL||len<0)
        return false;
    char *txdata22=txdata;
    for(int i=0;i<len;i++)
    {
        if(txdata[i]=='m'&&
           txdata[i+1]=='i'&&
           txdata[i+2]=='c'&&
           txdata[i+3]=='_')//  if(txdata[9]=='\n'数据    or  =='h'心跳
        {
            txdata[i]=0;
            txdata[i+1]=0;
            txdata[i+2]=0;
            txdata[i+3]=0;
            txdata[i+4]=0;
            txdata[i+5]=0;
            txdata[i+6]=0;
            txdata[i+7]=0;
            txdata[i+8]=0;
            txdata[i+9]=0;
        }
    }
    return false;
}
