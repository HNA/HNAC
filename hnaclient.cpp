#include "hnaclient.h"

#include <QSslSocket>
#include <QHostAddress>
#include <QFile>
#include <QStringList>

HnaClient::HnaClient(QObject *parent) :
    QObject(parent)
{
    settings = new QSettings(QSettings::SystemScope, "Hostel2", "hnac");
    m_login = settings->value("login", "").toString();
    m_pass = settings->value("password", "").toString();
    sock = new QSslSocket(this);

    expectedSslErrors.append(QSslError(QSslError::HostNameMismatch));

    connect(sock, SIGNAL(connected()), this, SLOT(sockConnected()));
    connect(sock, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(sockError(QAbstractSocket::SocketError)));
    connect(sock, SIGNAL(readyRead()), this, SLOT(sockReadyRead()));
    connect(sock, SIGNAL(encrypted()), this, SLOT(sockEncrypted()));
    connect(sock, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(sockSslErrors(QList<QSslError>)));

    QFile file(":/certs/hna.crt");
    file.open(QIODevice::ReadOnly);
    certsCA.append(QSslCertificate(file.readAll()));
    file.close();
    sock->setCaCertificates(certsCA);

    m_tmr = new QTimer(this);
    m_tmr->setInterval(15000);
    connect(m_tmr, SIGNAL(timeout()), this, SLOT(sockSendPing()));
    m_tmr->start();
    r_tmr = new QTimer(this);
    r_tmr->setInterval(1000);
    connect(r_tmr, SIGNAL(timeout()), this, SLOT(tryLogin()));
}

HnaClient::~HnaClient()
{}

void HnaClient::sockSendPing()
{
    sock->write("~ping;");
}

void HnaClient::tryLogin(QString login, QString pass)
{
    if(login.length() > 0)
        {
            m_login = login;
        }
    else
        {
            m_login = settings->value("login", "").toString();
        }
    if(pass.length() > 0)
        {
            m_pass = pass;
        }
    else
        {
            m_pass = settings->value("password", "").toString();
        }
    r_tmr->stop();
    sock->connectToHostEncrypted("172.30.0.1", 3816, QIODevice::ReadWrite);
}

void HnaClient::tryLogin()
{
    tryLogin(m_login, m_pass);
}

void HnaClient::sockEncrypted()
{
    loginQuery.clear();
    loginQuery.append("~login ")
            .append(m_login)
            .append(" ")
            .append(m_pass)
            .append(" ")
            .append(QString::number(sock->localAddress().toIPv4Address()))
            .append(";");
    sock->write(loginQuery);
}

void HnaClient::sockConnected()
{
        settings->setValue("login", m_login);
        settings->setValue("password", m_pass);
}

void HnaClient::sockError(QAbstractSocket::SocketError)
{
    r_tmr->start();
}

void HnaClient::logout()
{
    sock->write("~disconnect;");
    sock->disconnectFromHost();
}

void HnaClient::sockReadyRead()
{
    sock->readAll();//работать будет... но в случае шибки сложно отследить... позже исправлю..
}

void HnaClient::sockSslErrors(QList<QSslError> le)
{
    if (le.size() == 1 && le[0].error() == QSslError::HostNameMismatch)
    {
       ((QSslSocket*)sender())->ignoreSslErrors();
    }
}
