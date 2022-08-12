#ifndef FORMSHOW_H
#define FORMSHOW_H

#include <QWidget>

namespace Ui {
class FormShow;
}

class FormShow : public QWidget
{
    Q_OBJECT

public:
    explicit FormShow(QWidget *parent = nullptr);
    ~FormShow();

    void paintEvent(QPaintEvent *event);
    void setChannel(QString channel);
    void setAllRadio();
private slots:
    void on_pushButton_clicked();

private:
    Ui::FormShow *ui;
signals:
    void shootClose();
};

#endif // FORMSHOW_H
