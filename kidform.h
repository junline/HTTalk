#ifndef KIDFORM_H
#define KIDFORM_H

#include "formshow.h"

#include <QWidget>

#include <AudioDevice/inputsound.h>
#include <AudioDevice/receivesound.h>

namespace Ui {
class kidForm;
}

class kidForm : public QWidget
{
    Q_OBJECT
public:
    explicit kidForm(QWidget *parent = nullptr);
    ~kidForm();
    void setKid(QString ip, QString port, QString name);
    int hideTimes;
    QString m_ip,
    m_port,
    m_name;
    FormShow*ne;
    void paintEvent(QPaintEvent *event);
    void setShowImg(bool isShow, int times=10);
private slots:
    void on_pushButton_clicked();
    void receiveClose();
    void on_pushButton_clicked(bool checked);

private:
    Ui::kidForm *ui;
};

#endif // KIDFORM_H
