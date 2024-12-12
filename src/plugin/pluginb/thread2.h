#ifndef SubThread2_H
#define SubThread2_H

#include <QThread>
#include <QObject>
#include <lbase/lbase.h>
#include <QDebug>

class SubThread2 : public QThread
{
    Q_OBJECT
public:
    SubThread2(QObject * parent = nullptr);
    ~SubThread2();
    void run() override;
};

class TestClass : public QObject
{
    Q_OBJECT
public:
    TestClass(QObject * parent = nullptr)
        : QObject(parent)
    {
        // LEventBus::instance().subscribe("test_event", &TestClass::testSlots, this);
        LEventBus::instance().subscribe("test_event", SLOT(testSlots), this);
        qDebug() << "&TestClass::testSlots: " << &TestClass::testSlots;
    }
    ~TestClass()
    {
        LEventBus::instance().unSubscribe("test_event", SLOT(testSlots), this);
    };

public slots:
    Q_INVOKABLE void testSlots(const QVariant & var)
    {
        qDebug() << "testSlots_现成id:" << QThread::currentThreadId();
        qDebug() << var.toString();
    }
};

#endif
