#include <QDebug>
#include <QAction>
#include <QLabel>
#include <QStatusBar>
#include <QThread>

#include <lbase/lbase.h>

#include <common/CircularQueue.h>
#include <interface/iappskeleton/iappview.h>

#include "plugina.h"
#include "testclass.h"

PluginA::PluginA(QObject * parent)
    : QObject(parent)
{
}

QString PluginA::getname()
{
    return QString("插件A");
}

bool PluginA::init()
{
    m_actionTestClass = new QAction(QString("按钮1"));

    connect(m_actionTestClass, &QAction::triggered, this, &PluginA::addTestClass);
    // IPluginView::getInstance().registerAction(QString("组"), QString("页"), m_actionTestClass);  //todo,修改为基于抽象
    IAppView * baseObj = dynamic_cast<IAppView *>(ObjectRegistry::instance().getObject("IPluginView"));
    if (baseObj)
    {
        baseObj->registerAction(QString("组"), QString("页"), m_actionTestClass);
    }

    m_pt1 = new SubThread1(this);
    m_pt2 = new SubThread2(this);
    // m_pt1->start();
    m_pt2->start();
    return true;
}

bool PluginA::clean()
{
    return true;
}

void PluginA::addTestClass()
{
    qDebug() << "主线程id:" << QThread::currentThreadId();
    QVariant var("test");
    LEventBus::instance().publish("test_event", var);

    QThread::sleep(2);
    if (m_pt2)
    {
        delete m_pt2;
        m_pt2 = nullptr;
    }
}
