#include "kidform.h"
#include "mainwidget.h"
#include "mainwidget.h"
#include "mapXml.h"
#include "ui_mainwidget.h"
#include <QDesktopWidget>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkRequest>
#include <qdebug.h>
#include <QJsonArray>
#include <QCloseEvent>
#include "AudioDevice/_udpsocket.h"

mainWidget *mwnd=nullptr;
QString mainWidget::HttpheaderVoiceList ="/app/equip/getVoiceList";                     //接口
QString mainWidget::HttpheaderReceive   ="/app/soapi/SoApiWjsEquip/getLightOverTime";   //接口1
QString mainWidget::HttpheaderSender    ="/app/soapi/SoApiWjsEquip/setLightOverTime";   //后面接"/10"或"/5"、"/3" //接口2
QString mainWidget::Httpheader="";
//
QString mainWidget::UdpSocketIP;      //UdpSocketIP
int mainWidget::UdpSocketPort;      //UdpSocketPort

//QString mainWidget::HttpheaderXT      ="http://www.oxueo.cn";         //接口

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWidget)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("://titles.png"));
    setWindowTitle("广播对讲系统");

    mainWidget::Httpheader="http://"+
            Config().Get("IPCONFIG","IP").toString()+":"+
            Config().Get("IPCONFIG","Port").toString();

    mainWidget::UdpSocketIP=Config().Get("IPCONFIG","UdpIP").toString();
    mainWidget::UdpSocketPort=Config().Get("IPCONFIG","UdpPort").toInt();
    mainWidget::HttpheaderVoiceList =mainWidget::Httpheader+mainWidget::HttpheaderVoiceList;
    mainWidget::HttpheaderReceive   =mainWidget::Httpheader+mainWidget::HttpheaderReceive;
    mainWidget::HttpheaderSender    =mainWidget::Httpheader+mainWidget::HttpheaderSender;

    qDebug()<<">>"<<mainWidget::HttpheaderVoiceList;
    qDebug()<<"udpIP:"<<mainWidget::UdpSocketIP;
    qDebug()<<"udpPort:"<<mainWidget::UdpSocketPort;
//    ui->label_ip->setText("Server "+mainWidget::HttpheaderVoiceList);
    if(1==Config().Get("IPCONFIG","HttpHide").toInt())
    {
        ui->label_ip->setText(" ");
    }else
    {        
//        ui->label_ip->setText(" ");
        ui->label_ip->setText(mainWidget::HttpheaderVoiceList);
    }
    QDesktopWidget *wd=QApplication::desktop();
    //  wd->screenGeometry().width();     全屏幕大小  （包括任务栏）
    //  wd->availableGeometry().width();  可用桌面大小（不包括任务栏）
    move(wd->availableGeometry().width()-this->width(),wd->availableGeometry().height()-this->height()-30);//边框30
    ne=new FormShow();
    ne->hide();
    connect(ne,&FormShow::shootClose,this,&mainWidget::receiveClose);
    //全部列表
    QJsonObject json_object;
    setClassUrl(json_object,mainWidget::HttpheaderVoiceList);
    //对讲
    inputDevice=new InputSound(this,
                               mainWidget::UdpSocketIP,
                               mainWidget::UdpSocketPort);
    rSound=new ReceiveSound;
    connect(rSound,&ReceiveSound::shootIp,this,&mainWidget::receiveIp);//托盘事件

    //打开接收语音
    callStop();
    //小图标托盘
    quitAction.setText("对讲退出");
    quitAction.setParent(this);//右键点图标之后的选项
    myMenu.setParent((QWidget*)QApplication::desktop());
    myMenu.addAction(&quitAction);
    //判断系统是否支持托盘图标
    if(!QSystemTrayIcon::isSystemTrayAvailable())
    {
        return;
    }
    myTrayIcon.setParent(this);
    myTrayIcon.setIcon(QIcon(":/titles.png"));
    myTrayIcon.setToolTip("对讲系统");
    myTrayIcon.showMessage("托盘","托盘管理",QSystemTrayIcon::Information,10000);
    myTrayIcon.setContextMenu(&myMenu);
    myTrayIcon.show();
    connect(&myTrayIcon,&QSystemTrayIcon::activated,this,&mainWidget::activated);//托盘事件
    connect(&quitAction,&QAction::triggered,this,&mainWidget::CLOSE_S);//托盘退出
    tId=startTimer(1000);

    ui->comboBox->addItem("红绿灯超时设定10分钟");
    ui->comboBox->addItem("红绿灯超时设定5分钟");
    ui->comboBox->addItem("红绿灯超时设定3分钟");

//    HttpheaderSender;
    QJsonObject json_object2;
    setClassUrl(json_object2,mainWidget::HttpheaderReceive);
//    activateWindow();
    qDebug()<<"Start Began";

}
void mainWidget::receiveIp(QString ip)
{
    foreach(auto val,lKid)
    {
        if(ip == val->m_ip)
        {
            val->setShowImg(true,10);//計數正常10s后隐藏小喇叭
        }
    }
}

void mainWidget::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==tId)
    {
        foreach(auto val,lKid)
        {
            if(val->hideTimes>= 0)//計數
            {
                if(val->hideTimes<=0)
                    val->setShowImg(false);
                val->hideTimes--;
                qDebug()<<"消显操作计数"<<val->hideTimes<<val->m_ip;
            }
        }

    }
}
void mainWidget::closeEvent(QCloseEvent *event)
{
    myUdpSocket->stopUdp();
    myTrayIcon.hide();
    myTrayIcon.setVisible(false);
    this->close();//退出
    qDebug()<<">>>>closeEvent:return"<<event->type();

//        event->ignore();
}
void mainWidget::CLOSE_S(bool bo)
{
//    myTrayIcon.deleteLater();
    this->close();//退出
}

void mainWidget::activated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger :
//        this->setWindowFlags(Qt::Tool);
//        this->hide();
//        qDebug()<<windowFlags();
//        if(windowFlags()!=(Qt::FramelessWindowHint|Qt::Window))
//        {
//            qDebug()<<"FramelessWindowHint s"<<windowFlags();
//            this->setWindowFlags(Qt::FramelessWindowHint);
//        }
//        if(windowState()!=Qt::WindowMaximized)
//        {
//            this->setWindowState(Qt::WindowMaximized); //最大化
//        }
        showNormal();
        activateWindow();//窗口置顶
        break;
    }
}

mainWidget::~mainWidget()
{
    delete ui;
}

void mainWidget::addKidWidget(QString ip,QString name,QString port)
{
    kidForm *kid=new kidForm;
    kid->setKid(ip,port,name);
    ui->verticalLayoutCenter->insertWidget(0,kid);
    lKid.append(kid);
//    qDebug()<<"post:list2:"<<ip;
}

//全部
void mainWidget::on_pushButton_7_clicked(bool checked)
{
    if(checked)
    {
        foreach(auto val,lKid)
        {
            lKid.removeOne(val);
            ui->verticalLayoutCenter->removeWidget(val);
            val->deleteLater();
        }
        QJsonObject json_object;
        setClassUrl(json_object,mainWidget::HttpheaderVoiceList);
        ui->pushButton_8->setChecked(false);
        ui->pushButton_9->setChecked(false);
    }else
    {
        ui->pushButton_7->setChecked(true);
    }
}

//车载机
void mainWidget::on_pushButton_8_clicked(bool checked)
{
    if(checked)
    {
        foreach(auto val,lKid)
        {
            lKid.removeOne(val);
            ui->verticalLayoutCenter->removeWidget(val);
            val->deleteLater();
        }
        QJsonObject json_object;
//        type (integer, optional): 1基站2对讲机3车载机
        json_object.insert("type",3);
        setClassUrl(json_object,mainWidget::HttpheaderVoiceList);
        ui->pushButton_7->setChecked(false);
        ui->pushButton_9->setChecked(false);
    }else
    {
        ui->pushButton_8->setChecked(true);
    }
}
//广播分站
void mainWidget::on_pushButton_9_clicked(bool checked)
{
    if(checked)
    {
        foreach(auto val,lKid)
        {
            lKid.removeOne(val);
            ui->verticalLayoutCenter->removeWidget(val);
            val->deleteLater();
        }
        QJsonObject json_object;
//      type (integer, optional): 1基站2对讲机3车载机
        json_object.insert("type",1);
        setClassUrl(json_object,mainWidget::HttpheaderVoiceList);
        ui->pushButton_8->setChecked(false);
        ui->pushButton_7->setChecked(false);
    }else
    {
        ui->pushButton_9->setChecked(true);
    }
}

//全局广播
void mainWidget::on_pushButton_clicked()
{
    ne->setAllRadio();
    ne->setChannel("正在进行全局广播");
    ne->show();
    ne->move(
             mwnd->pos().x()+mwnd->width()/2 -ne->width()/2,
             mwnd->pos().y()+mwnd->height()/2-ne->height()/2
            );
    foreach (auto val,lKid)
    {
        val->setShowImg(true,1000);//計數正常1000s后隐藏小喇叭
    }

    rSound->StopUdp();
    inputDevice->StartAudio("",5008);

}
void mainWidget::on_pushButton_clicked(bool checked)
{

}
//全局广播 取消
void mainWidget::receiveClose()
{
    ui->pushButton->setChecked(false);

    foreach (auto val,lKid){
        val->setShowImg(false);
    }
    callStop();
}

//接收语音，停止播放
void mainWidget::callStop()
{
    inputDevice->StopAudio();
    rSound->StartUdp();
    qDebug()<<"post1111打开接收语音，停止播放";
}

//开始对讲,停止接收
void mainWidget::callIpPort(QString ip,int port)
{
    rSound->StopUdp();
    inputDevice->StartAudio(ip,port);
}

//请求服务器列表
void mainWidget::setClassUrl(QJsonObject json_object,QString url)
{
    QByteArray postData = QJsonDocument(json_object).toJson();
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Content-Type","application/json");
    request.setRawHeader("token","");//studentInfo::toKen.toUtf8()

    reply = manager.post(request,postData);
    QEventLoop loop;
    connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();

    if(reply==Q_NULLPTR)
    {
        qDebug() << "post:3 reply is NULL"<<endl;
        reply->deleteLater();
        return;
    }
    QByteArray bytes = reply->readAll();
    reply->deleteLater();
    reply = Q_NULLPTR;
    qDebug() << "post: reply is NULL"<<bytes;
    //json解析
    QJsonParseError err_rpt;
    QJsonDocument   root_Doc = QJsonDocument::fromJson(bytes, &err_rpt);//字符串格式化为JSON
    lstr.clear();
    if(err_rpt.error != QJsonParseError::NoError)
    {
        qDebug()<< "post:JSON1 格式不匹配";
        return;
    }else//JSON格式正确
    {
        QJsonObject root_Obj=root_Doc.object();
        int code=  root_Obj.value("code").toInt();
        QString msg=   root_Obj.value("msg").toString();
        QString actionTag=   root_Obj.value("actionTag").toString();
        int data=   root_Obj.value("data").toInt();
        //返回指令
        if(actionTag=="/app/soapi/SoApiWjsEquip/getLightOverTime" && code==0)
        {
            qDebug()<<"getLightOverTime"<<data;
            if(data==10)
            {   //红绿灯超时设定10分钟
                ui->comboBox->setCurrentIndex(0);
            }else if(data==5)
            {   //红绿灯超时设定5分钟
                ui->comboBox->setCurrentIndex(1);
            }else if(data==3)
            {   //红绿灯超时设定3分钟
                ui->comboBox->setCurrentIndex(2);
            }
        }
        else if(actionTag=="/app/soapi/SoApiWjsEquip/setLightOverTime/10" && code==0)
        {
            ui->label_ip->setText("已设定10分钟");
        }
        else if(actionTag=="/app/soapi/SoApiWjsEquip/setLightOverTime/5" && code==0)
        {
            ui->label_ip->setText("已设定5分钟");
        }
        else if(actionTag=="/app/soapi/SoApiWjsEquip/setLightOverTime/3" && code==0)
        {
            ui->label_ip->setText("已设定3分钟");
        }
        else if(msg=="success")//
        {
            qDebug()<< "post:success";
            QJsonArray data        =   root_Obj.value("data").toArray();
            for(int i=0;i<data.count();i++)
            {
                QStringList qlist;
                QJsonObject obj = data.at(i).toObject();
                qlist<<       obj.value("ip").toString()
                     <<       obj.value("name").toString()
                     <<       obj.value("port").toString();
                lstr.append(qlist);
            }
        }
    }
    for(int i=0;i<lstr.count();i++)
    {
        qDebug()<<"post:list:"<<i<<lstr.at(i);
        addKidWidget(lstr.at(i).at(0),
                     lstr.at(i).at(1),
                     lstr.at(i).at(2));
    }
}

void mainWidget::on_pushButton_7_clicked()
{

}

void mainWidget::on_comboBox_activated(const QString &arg1)
{
    if(arg1=="红绿灯超时设定10分钟")
    {
        qDebug()<<arg1;
        QJsonObject json_object2;
        setClassUrl(json_object2,mainWidget::HttpheaderSender+"/10");
    }
    if(arg1=="红绿灯超时设定5分钟")
    {
        qDebug()<<arg1;
        QJsonObject json_object2;
        setClassUrl(json_object2,mainWidget::HttpheaderSender+"/5");
    }
    if(arg1=="红绿灯超时设定3分钟")
    {
        qDebug()<<arg1;
        QJsonObject json_object2;
        setClassUrl(json_object2,mainWidget::HttpheaderSender+"/3");
    }
}
