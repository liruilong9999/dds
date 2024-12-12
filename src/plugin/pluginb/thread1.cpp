#include <list>
#include <memory>
#include <iostream>
#include <lbase/lbase.h>
#include <QThread>
#include <QVariant>
#include <QDebug>

#include "thread1.h"

SubThread1::SubThread1(QObject * parent /*= nullptr*/)
    : QThread(parent)
{
}

SubThread1::~SubThread1()
{
    quit();
    // requestInterruption();
    wait();
}

void SubThread1::run()
{
    qDebug() << "子线程1线程id为：" << QThread::currentThreadId();

    exec();
}
