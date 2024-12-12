#ifndef APPSKELETONPLUGIN_H__20241109
#define APPSKELETONPLUGIN_H__20241109

#include <QObject>
#include <QtPlugin>
#include <QAction>
#include <common/IPlugin.h>

class IPluginView;
class QMainWindow;

class AppSkeletonPlugin : public QObject,
                          public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "AppSkeletonPlugin" FILE "plugin_info.json") // QT5.0 ÒýÈë
    Q_INTERFACES(IPlugin)
public:
    explicit AppSkeletonPlugin(QObject * parent = 0);
    virtual QString getname() override;
    virtual bool    init() override;
    virtual bool    clean() override;

private:
    IPluginView * m_pIPluginView{nullptr};
};

#endif
