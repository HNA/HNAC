#include "service.h"
#include "hnaclient.h"

Service::Service(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, "Hna Client")
{
    if(argc == 3)
        {
            m_login = argv[1];
            m_pass = argv[2];
        }
    setServiceDescription("HNA Client service");
    setServiceFlags(QtServiceBase::CanBeSuspended);
}

void Service::start()
{
    QCoreApplication *app = application();
    c = new HnaClient(app);
    c->tryLogin(m_login, m_pass);
}

void Service::stop()
{
    c->logout();
}

void Service::resume()
{
    c->tryLogin();
}
