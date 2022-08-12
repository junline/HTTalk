#ifndef MAPXML_H
#define MAPXML_H

#include <QString>
#include <QVariant>
#include <QSettings>

#pragma execution_character_set("utf-8")
//产品类型
//0001基站
//0002广播分站
//0003车载机
//0004LED显示屏
//0005编码器
//0006道岔控制器
//0007读卡器
//0008红绿灯
//0009红外微波探测模块
//000A手持机
//000B尾灯
//000C自动停送电
//000D摄像头

//#include "TcpData/tcptable.h"
class TcpClientSocket;
//报文信息
struct DevInfo
{
        QString    bwType;//报文类型
        QString    Id;//产品ID
        QString    DevType;//产品类型标识
        QString    DevTypeName;//产品类型标识名称
        QString    DevObjSqlName;//数据库设备名称
        QString    DataType;//数据类型
        QString    bwData;//报文数据内容
        QString    sendTiem;//发送时间
        TcpClientSocket*mySocket;
};
Q_DECLARE_METATYPE(DevInfo)

//地图属性信息
struct mapInfo
{
    int ID;                 //item类型  0-铁轨 1-道岔 2-000A手持机   3-000D摄像头   4-0008红绿灯   5-文字   6-0004LED显示屏   7-000C自动停送电  8-0005编码器 9-0007读卡器
    float rotate;             //角度
    float rotate2;            //道岔角度2
    int height;             //寬高
    int width;
    int total;              //铁轨节数
    int zOrder;
    int ItemNumber;         //增加的数量
    float posx;             //位置
    float posy;
    QString hint;           //提示信息
    QString text;           //文字
    //add at 0416
    QString IP;
    int Port;
    int Vchannel;           //摄像头播放通道
    QString Vname;          //摄像头名称
    QString Vpassword;      //摄像头密码
    //add at 0516
    QString DevName;        //产品名称
    QString DevID;          //产品ID
    QString Id_Type;        //产品ID_type
    QString DevType;        //产品类型标识
    int DevID_int;      //产品 int ID
    int DevType_int;    //产品类型标识 int 类型
    QString DevTeamID;  //zu id
    int equipStatus;    //设备状态1在线2离线3故障4报警
    int authType;       //1合法  0非法
  //add at 20210206策略项
    int pid,idx,mapType;//20210206策略项
  //add at 0224
    double destination;       //铁道距离参数
};
Q_DECLARE_METATYPE(mapInfo)

extern mapInfo tempMapInfo;     //临时图元信息
extern QList<mapInfo> lMapInfo; //地图图元信息集合
extern QList<mapInfo> equipInfo;//设备信息集合
extern int changeTypeRailway; //Railway切换移动方式
class mapXml
{
public:
    mapXml();
    static void readDom(QString);   //读取地图
    static void writeDom(QString);  //保存写入
    static void writeLog(QString str, QString LogType);
};

class Config
{
public:
    Config(QString qstrfilename = "");
    virtual ~Config(void);
    void Set(QString,QString,QVariant);
    QVariant Get(QString,QString);
private:
    QString m_qstrFileName;
    QSettings *m_psetting;
};
#endif // MAPXML_H
