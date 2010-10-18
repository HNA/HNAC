#ifndef SERVICE_H
#define SERVICE_H

#include <QCoreApplication>

#include "../service/qtservice.h"

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
    QString m_login, m_pass;
};

#endif // SERVICE_H
