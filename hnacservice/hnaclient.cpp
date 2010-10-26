#include "hnaclient.h"

#include <QSslSocket>
#include <QHostAddress>
#include <QFile>
#include <QStringList>
#include <QSettings>
#include <QTimer>

HnaClient::HnaClient(QObject *parent) :
    QObject(parent)
{
    settings = new QSettings(QSettings::SystemScope, "Hostel2", "hnac");
#if !defined(Q_WS_WIN)
    m_login = settings->value("login", "").toString();
    m_pass = settings->value("password", "").toString();
    newData = false;
#endif
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
    r_tmr->setInterval(5000);
    connect(r_tmr, SIGNAL(timeout()), this, SLOT(tryLogin()));
}

HnaClient::~HnaClient()
{}

void HnaClient::sockSendPing()
{
    sock->write("~ping;");
}

#if !defined(Q_WS_WIN)
void HnaClient::tryLogin(QString login, QString pass)
{
        m_login = login;
        m_pass = pass;
        newData = true;
        r_tmr->stop();
        sock->connectToHostEncrypted("172.30.0.1", 3816, QIODevice::ReadWrite);
}
#endif

void HnaClient::tryLogin()
{
#if defined(Q_WS_WIN)
    m_login = settings->value("login", "").toString();
    m_pass = settings->value("password", "").toString();
#endif
    r_tmr->stop();
    sock->connectToHostEncrypted("172.30.0.1", 3816, QIODevice::ReadWrite);
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
    // у некоторых баги с этой штукой (скорее всего функция выдает не тот IP... надо сделать проверку
}

void HnaClient::sockConnected()
{
#if !defined(Q_WS_WIN)
    if(newData)
        {
            settings->setValue("login", m_login);
            settings->setValue("password", m_pass);
        }
#endif
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
    sock->readAll();//работать будет... но в случае ошибки сложно отследить... позже исправлю..
}

void HnaClient::sockSslErrors(QList<QSslError> le)
{
    if (le.size() == 1 && le[0].error() == QSslError::HostNameMismatch)
    {
       ((QSslSocket*)sender())->ignoreSslErrors();
    }
}
