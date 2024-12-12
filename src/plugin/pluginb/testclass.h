#ifndef TESTCLASS2_H
#define TESTCLASS2_H

#include <QObject>
#include <lbase/lbase.h>
#include <lbase/llog.h>

#include <QDebug>

class testClass : public QObject
{
    Q_OBJECT
public:
    // ReflectionRegistry * res = ReflectionRegistry::getInstance();
    virtual void print()
    {
        qDebug() << "testClass1";
    };
};
REGISTER(testClass, "testClassStr"); // 仅注册类，不指定模式

class testClass2 : public testClass
{
    Q_OBJECT
public:
    // ReflectionRegistry * res = ReflectionRegistry::getInstance();
    void print() override
    {
        qDebug() << "testClass2:" << m_testNumber;
        m_testNumber++;
    };

private:
    int m_testNumber{0};
};
REGISTER(testClass2, "testClassStr2"); // 仅注册类，不指定模式

#endif
