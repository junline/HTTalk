#include "mainwidget.h"
#include <QApplication>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QMessageBox>

int main(int argc, char *argv[])
{
   // Set small icon

    QApplication a(argc, argv);

    //同一個程序不能運行第二個
    QSystemSemaphore semaphore("ProgramKey",1, QSystemSemaphore::Open);
    semaphore.acquire();
    //在临界区操作共享内存SharedMemory
    QSharedMemory memory("programCarSystem");//全局对象名
    if(!memory.create(1))//如果全局对象存在则提示退出
    {
        QMessageBox::information(0, "提示消息", "已有程序在运行，请先关闭当前运行程序!");
        semaphore.release();
        return 0;
    }
    semaphore.release();

    mwnd=new mainWidget;
    mwnd->show();
    return a.exec();
}


