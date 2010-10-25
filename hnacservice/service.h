#ifndef SERVICE_H
#define SERVICE_H

#include <QCoreApplication>

#include "qtservice.h"

class HnaClient;

class Service : public QtService<QCoreApplication>
{
public:
    Service(int argc, char **argv);

protected:
    void start();
    void stop();
    void resume();

private:
    HnaClient *c;
#if !defined(Q_WS_WIN)
    QString m_login, m_pass;
    bool newData;
#endif
};

#endif // SERVICE_H
