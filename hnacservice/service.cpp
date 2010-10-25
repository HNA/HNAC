#include "service.h"
#include "hnaclient.h"

Service::Service(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, "Hna Client")
{
#if !defined(Q_WS_WIN)
    if(argc == 3)
        {
            m_login = argv[1];
            m_pass = argv[2];
            newData = true;
        }
    else
    {
        newData = false;
    }
#endif
    setServiceDescription("HNA Client service");
}

void Service::start()
{
    QCoreApplication *app = application();
    c = new HnaClient(app);
#if defined(Q_WS_WIN)
    c->tryLogin();
#else
    if(newData)
    {
        c->tryLogin(m_login, m_pass);
    }
    else
    {
        c->tryLogin();
    }
#endif
}

void Service::stop()
{
    c->logout();
}

void Service::resume()
{
    c->tryLogin();
}
