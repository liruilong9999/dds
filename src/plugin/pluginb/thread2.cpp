
#include <list>
#include <memory>
#include <iostream>
#include <lbase/lbase.h>
#include <QThread>
#include <QVariant>
#include <QDebug>
#include "thread2.h"

SubThread2::SubThread2(QObject * parent /*= nullptr*/)
    : QThread(parent)
{
}

SubThread2::~SubThread2()
{
    quit();
    // requestInterruption();
    wait();
}

void SubThread2::run()
{

    qDebug() << "子线程2线程id为：" << QThread::currentThreadId();
    TestClass test; // = new TestClass;
    // QObject::connect(test,&TestClass::testSlots);
    exec();
    // delete test;
}
