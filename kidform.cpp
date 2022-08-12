#include "formshow.h"
#include "kidform.h"
#include "ui_kidform.h"
#include "mainwidget.h"
#include "qdebug.h"

#include <QPainter>
#include <QStyleOption>

kidForm::kidForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::kidForm)
{
    ui->setupUi(this);
    ui->pushButton->setText("对讲");
    ne=new FormShow();
    connect(ne,&FormShow::shootClose,this,&kidForm::receiveClose);
    ui->label_img->hide();
    hideTimes=0;
}

kidForm::~kidForm()
{
    delete ui;
}

void kidForm::setKid(QString ip,QString port,QString name)
{
     m_ip=ip;
     m_port=port;
     m_name=name;
     ui->label_ip->setText(ip);
     ui->label_port->setText(port);
     ui->label_devName->setText(name);
}

void kidForm::on_pushButton_clicked()
{
    ui->pushButton->setText("对讲中..");    
    setShowImg(true,1000);//計數正常1000s后隐藏小喇叭

    ne->setChannel(tr("频道：%1").arg(m_port));
    ne->show();
    ne->move(mwnd->pos().x()+mwnd->width()/2 -ne->width()/2,
             mwnd->pos().y()+mwnd->height()/2-ne->height()/2);
    //开始对讲
    mwnd->callIpPort(m_ip,m_port.toInt());
}

//停止对讲
void kidForm::receiveClose()
{
    ui->pushButton->setText("对讲");
    ui->pushButton->setChecked(false);
    setShowImg(false);
    //打开接收语音，停止播放
    mwnd->callStop();
}

void kidForm::setShowImg(bool isShow,int times)
{
    if(isShow)
    {
        ui->label_img->show();
        ui->label_ip->setStyleSheet("color: rgb(0, 205, 0);");
        hideTimes=times;
    }
    else
    {
            ui->label_img->hide();
            ui->label_ip->setStyleSheet("color: rgb(0, 0, 0);");
    }
    qDebug()<<"消显操作"<<isShow<<times<<m_ip<<hideTimes;
}

void kidForm::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void kidForm::on_pushButton_clicked(bool checked)
{

}
