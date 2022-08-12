#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "formshow.h"
#include "kidform.h"
#include <QMenu>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSystemTrayIcon>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class mainWidget; }
QT_END_NAMESPACE

class mainWidget : public QWidget
{
    Q_OBJECT
public:
    mainWidget(QWidget *parent = nullptr);
    ~mainWidget();

    QAction quitAction;
    QMenu myMenu;
    void addKidWidget(QString ip, QString name, QString port);
    FormShow*ne;
    QList<kidForm*>lKid;

    QNetworkReply *reply;
    QNetworkAccessManager manager;
    static QString HttpheaderVoiceList; //接口
    static QString HttpheaderReceive;   //接口1
    static QString HttpheaderSender;    //接口2
    static QString Httpheader;          //接口
    static QString UdpSocketIP;         //接口
    static int UdpSocketPort;           //接口
    void setClassUrl(QJsonObject json_object,QString qstr);
    InputSound* inputDevice;
    ReceiveSound*rSound;    
    int tId;
    QSystemTrayIcon myTrayIcon;

    void callIpPort(QString ip, int port);
    void callStop();
    void closeEvent(QCloseEvent *event);
    QList<QStringList> lstr;
    void timerEvent(QTimerEvent *event);
public slots:
    void CLOSE_S(bool bo);
    void activated(QSystemTrayIcon::ActivationReason reason);
    void receiveIp(QString ip);
private slots:
    void on_pushButton_7_clicked(bool checked);
    void on_pushButton_8_clicked(bool checked);
    void on_pushButton_9_clicked(bool checked);
    void on_pushButton_clicked();
    void on_pushButton_clicked(bool checked);
    void receiveClose();
    void on_pushButton_7_clicked();

    void on_comboBox_activated(const QString &arg1);

private:
    Ui::mainWidget *ui;
};

extern mainWidget *mwnd;

#endif // MAINWIDGET_H
