#include"mapXml.h"
#include<QDomDocument>
#include<QDomElement>
#include<QFile>
#include<QTextStream>
#include<iostream>
#include<qmessagebox.h>
#include<QDebug>
#include <QDir>
#include <QtCore/QtCore>
using namespace std;

QList<mapInfo> lMapInfo;
QList<mapInfo> equipInfo;//设备信息集合

mapInfo tempMapInfo;  //addItem临时储存
int changeTypeRailway=0; //Railway切换移动方式  0铁轨  1自由移动

mapXml::mapXml()
{
}

//写入信息到文件   WriteLog(text,"LogName_");
void mapXml::writeLog(QString str, QString LogType)
{

    QString fileFolder;
    fileFolder= qApp->applicationDirPath()+"/log/"+QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QDir dir(fileFolder);
    if(!dir.exists())
    {
        dir.mkpath(fileFolder);
    }
    LogType=LogType+QDateTime::currentDateTime().toString("_hh");

    QString filePath=QString("%1/%2.log").arg(fileFolder).arg(LogType);

    QString strToWrite=QDateTime::currentDateTime().toString("hhmmss");
    strToWrite.append(QString(" %1").arg(str));
//  strToWrite.append("\r\n---------------------------------------------------------------------");
    QFile file(filePath);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream <<strToWrite<<"\r\n";
    file.flush();
    file.close();
    //打印到控制台
//    std::cout << strToWrite.toLocal8Bit().constData() << std::endl;
}
void mapXml::writeDom(QString str)
{
    QFile file(str);
    if(!file.open(QFile::WriteOnly|QFile::Truncate)) //可以用QIODevice，Truncate表示清空原来的内容
    {
        QMessageBox::information(NULL,"Error","xml file is open failed");
        return;
    }
    QDomDocument doc;
    //写入xml头部
    QDomProcessingInstruction instruction;
    instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    //添加根节点
    QDomElement originRoot = doc.createElement("MapInfo");
    doc.appendChild(originRoot);

    for(int i=0;i<lMapInfo.count();i++)
    {
        QDomElement Elements = doc.createElement("Item");
        Elements.setAttribute("ID",     lMapInfo.at(i).ID);
        Elements.setAttribute("posx",   lMapInfo.at(i).posx);
        Elements.setAttribute("posy",   lMapInfo.at(i).posy);
        Elements.setAttribute("rotate", lMapInfo.at(i).rotate);
        Elements.setAttribute("hint",   lMapInfo.at(i).hint);
        Elements.setAttribute("total",  lMapInfo.at(i).total);
        Elements.setAttribute("height", lMapInfo.at(i).height);
        Elements.setAttribute("width",  lMapInfo.at(i).width);
        Elements.setAttribute("text",   lMapInfo.at(i).text);
        //add at 0416
        Elements.setAttribute("IP",       lMapInfo.at(i).IP);
        Elements.setAttribute("Port",     lMapInfo.at(i).Port);
        Elements.setAttribute("Vchannel", lMapInfo.at(i).Vchannel);
        Elements.setAttribute("Vname",    lMapInfo.at(i).Vname);
        Elements.setAttribute("Vpassword",lMapInfo.at(i).Vpassword);
        //add at 0516
        Elements.setAttribute("DevID",    lMapInfo.at(i).DevID);
        Elements.setAttribute("DevName",  lMapInfo.at(i).DevName);
        Elements.setAttribute("DevType",  lMapInfo.at(i).DevType);

        //add at 210206
        Elements.setAttribute("DevID",    lMapInfo.at(i).DevID);
        Elements.setAttribute("DevName",  lMapInfo.at(i).DevName);
        Elements.setAttribute("DevType",  lMapInfo.at(i).DevType);

//      pid,idx,mapType;//20210206策略项
        Elements.setAttribute("pid",        lMapInfo.at(i).pid);
        Elements.setAttribute("idx",        lMapInfo.at(i).idx);
        Elements.setAttribute("mapType",    lMapInfo.at(i).mapType);
        //add 20210707
        Elements.setAttribute("zOrder",    lMapInfo.at(i).zOrder);
        originRoot.appendChild(Elements);
    }
    QTextStream out(&file);
    doc.save(out,4); //缩进4格
    file.close();
}
void mapXml::readDom(QString str){

    QFile file(str);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL,"ReadOnly Error",str+"地图打开失败");
        return;
    }
    QDomDocument doc;
    doc.setContent(&file);

    QDomNodeList nodelist =doc.elementsByTagName("Item");
//  qDebug()<<"count:"<<nodelist.count();

    lMapInfo.clear();
    for(int i=0;i<nodelist.count();i++)
    {
        mapInfo mapInfoVal;
        QDomNode node       =nodelist.at(i);
        mapInfoVal.ID       =node.toElement().attribute("ID").toInt();
        mapInfoVal.posx     =node.toElement().attribute("posx").toFloat();
        mapInfoVal.posy     =node.toElement().attribute("posy").toFloat();
        mapInfoVal.rotate   =node.toElement().attribute("rotate").toInt();
        mapInfoVal.hint     =node.toElement().attribute("hint");
        mapInfoVal.total    =node.toElement().attribute("total").toInt();
        mapInfoVal.height   =node.toElement().attribute("height").toInt();
        mapInfoVal.width    =node.toElement().attribute("width").toInt();
        mapInfoVal.text     =node.toElement().attribute("text");
        //add at 0416
        mapInfoVal.IP           =node.toElement().attribute("IP");
        mapInfoVal.Port         =node.toElement().attribute("Port").toInt();
        mapInfoVal.Vchannel     =node.toElement().attribute("Vchannel").toInt();
        mapInfoVal.Vname        =node.toElement().attribute("Vname");
        mapInfoVal.Vpassword    =node.toElement().attribute("Vpassword");
        //add at 0516
        mapInfoVal.DevID        =node.toElement().attribute("DevID");
        mapInfoVal.DevName      =node.toElement().attribute("DevName");
        mapInfoVal.DevType      =node.toElement().attribute("DevType");
        //      pid,idx,mapType;//20210206策略项
        mapInfoVal.pid        =node.toElement().attribute("pid").toInt();
        mapInfoVal.idx      =node.toElement().attribute("idx").toInt();
        mapInfoVal.mapType      =node.toElement().attribute("mapType").toInt();
        //add 20210707
        mapInfoVal.zOrder = node.toElement().attribute("zOrder").toInt();

        lMapInfo.append(mapInfoVal);
    }
    file.close();
}

#include <QtCore/QtCore>

#include <QtXml/qdom.h>
Config::Config(QString qstrfilename)
{
    if (qstrfilename.isEmpty())
    {
//      m_qstrFileName = QCoreApplication::applicationDirPath() + "/Config.ini";
        m_qstrFileName = "./Config.ini";//读取system展台配置
    }
    else
    {
        m_qstrFileName = qstrfilename;
    }
    m_psetting = new QSettings(m_qstrFileName, QSettings::IniFormat);
    qDebug() <<"Config:"<<m_qstrFileName;
}

Config::~Config()
{
    delete m_psetting;
    m_psetting = 0;
}

void Config::Set(QString qstrnodename,QString qstrkeyname,QVariant qvarvalue)
{
    m_psetting->setValue(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname), qvarvalue);
}

QVariant Config::Get(QString qstrnodename,QString qstrkeyname)
{
    QVariant qvar = m_psetting->value(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname));
    return qvar;
}

//    QString qstrname = Config().Get("user","name").toString();
//    qDebug() << qstrname;
//    QString qstrpasswd = Config().Get("user","password").toString();
//    qDebug() << qstrpasswd;
//    int nport = Config().Get("test","port").toInt();
