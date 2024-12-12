
#ifndef PLUGINA_H
#define PLUGINA_H

#include <QObject>
#include <QtPlugin>
#include <QAction>
#include <common/IPlugin.h>

#include "thread1.h"
#include "thread2.h"

class TestClass;

class PluginA : public QObject,
                public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "PluginA" FILE "plugin_info.json") // QT5.0 引入
    Q_INTERFACES(IPlugin)
public:
    explicit PluginA(QObject * parent = 0);
    virtual QString getname() override;
    virtual bool    init() override;
    virtual bool    clean() override;
public slots:
    void addTestClass();

private:
    QAction * m_actionTestClass{nullptr}; // 按钮

    SubThread1 * m_pt1{nullptr};
    SubThread2 * m_pt2{nullptr};
};

#endif
