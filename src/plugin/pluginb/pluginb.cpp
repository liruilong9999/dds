#include <QDebug>
#include <QAction>
#include <QLabel>
#include <QStatusBar>
#include <QThread>

#include <lbase/lbase.h>

#include <common/CircularQueue.h>
#include <interface/iappskeleton/iappview.h>

#include "pluginb.h"
#include "testclass.h"

PluginB::PluginB(QObject * parent)
    : QObject(parent)
{
}

QString PluginB::getname()
{
    return QString("插件B");
}

bool PluginB::init()
{
    m_actionTestClass = new QAction(QString("按钮2"));

    connect(m_actionTestClass, &QAction::triggered, this, &PluginB::addTestClass);
    // IPluginView::getInstance().registerAction(QString("组"), QString("页"), m_actionTestClass);  //todo,修改为基于抽象
    IAppView * baseObj = dynamic_cast<IAppView *>(ObjectRegistry::instance().getObject("IPluginView"));
    if (baseObj)
    {
        baseObj->registerAction(QString("组"), QString("页"), m_actionTestClass);
    }
    return true;
}

bool PluginB::clean()
{
    return true;
}

void PluginB::addTestClass()
{
    QObject *    cla = ClassFactory::getInstance().getClassByName("testClassStr2");
    testClass *  tc1 = qobject_cast<testClass *>(cla);

	LOG_INFO("(PluginB):infotestpluginb")
    tc1->print();
}
