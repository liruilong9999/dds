#ifndef SubThread1_H
#define SubThread1_H

#include <QThread>

class SubThread1 : public QThread
{
    Q_OBJECT
public:
    SubThread1(QObject * parent = nullptr);
    ~SubThread1();
    void run() override;
};

#endif
