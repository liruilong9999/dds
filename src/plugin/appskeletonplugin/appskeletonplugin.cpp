#include <QDebug>
#include <QAction>
#include <QLabel>
#include <QStatusBar>
#include <QThread>

#include <lbase/lbase.h>

#include <common/CircularQueue.h>
#include "appskeleton.h"
#include "ipluginview.h"

#include "appskeletonplugin.h"

AppSkeletonPlugin::AppSkeletonPlugin(QObject * parent)
    : QObject(parent)
{
    m_pIPluginView = new IPluginView;

    // 将m_pIPluginView 注册
    ObjectRegistry::instance().registerObject("IPluginView", m_pIPluginView);
}

QString AppSkeletonPlugin::getname()
{
    return QString("主界面");
}

bool AppSkeletonPlugin::init()
{
    QMenuBar * menuBar = m_pIPluginView->getMainWindow()->menuBar();
    m_pIPluginView->createMenuBar(menuBar);
    m_pIPluginView->getMainWindow()->show();
    return true;
}

bool AppSkeletonPlugin::clean()
{
    if (m_pIPluginView)
    {
        delete m_pIPluginView;
        m_pIPluginView = nullptr;
    }

    return true;
}
