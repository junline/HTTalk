#include "formshow.h"
#include "ui_formshow.h"

#include <QPainter>
#include <QStyleOption>

FormShow::FormShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormShow)
{
    ui->setupUi(this);
    //  设置窗体透明
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    //设置无边框
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint|Qt::SubWindow);//SubWindow设置无任务栏图标
    this->setWindowModality(Qt::ApplicationModal);//阻止其它窗口置顶
}

FormShow::~FormShow()
{
    delete ui;
}

void FormShow::on_pushButton_clicked()
{
    emit shootClose();
    close();
}

void FormShow::setChannel(QString channel)
{
    ui->label_channel->setText(channel);
}

void FormShow::setAllRadio()
{
    this->ui->widget->setStyleSheet(
        "QWidget#widget"
        "{"
        "border-radius:20px;"
        "background-color: rgba(255, 85, 0,0.9);"
        "}");
    ui->pushButton->setText("结束广播");
    ui->label_2->setText("讲话中..");
}

void FormShow::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
