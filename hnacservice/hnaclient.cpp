#include "hnaclient.h"

#include <QSslSocket>
#include <QHostAddress>
#include <QFile>
#include <QStringList>
#include <QSettings>
#include <QTimer>
#include <QNetworkInterface>

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

//отсылаем пинг
    m_tmr = new QTimer(this);
    m_tmr->setInterval(15000);
    connect(m_tmr, SIGNAL(timeout()), this, SLOT(sockSendPing()));
    m_tmr->start();
//если ошибка...
    r_tmr = new QTimer(this);
    connect(r_tmr, SIGNAL(timeout()), this, SLOT(tryLogin()));
}

HnaClient::~HnaClient()
{}

void HnaClient::sockSendPing()
{
    if(sock->write("~ping;") < 0) //ошибка при записи
    {
        r_tmr->start(5000);
    }
}

void HnaClient::tryLogin(QString login, QString pass)
{
        if(login.isEmpty() && pass.isEmpty())
        {
            if(m_login.isEmpty() && m_pass.isEmpty())
            {
                m_login = settings->value("login", "").toString();
                m_pass = settings->value("password", "").toString();
            }
        }
        else
        {
            m_login = login;
            m_pass = pass;
            newData = true;
        }
        r_tmr->stop();

//проверка на то есть ли сеть вообще =)
//если нет - каждые 10 секунд пробовать переподключиться..
        QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        for (int i = 0; i < ipAddressesList.size(); ++i)
        {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address())
            {
                ipAddress = ipAddressesList.at(i).toString();
                break;
            }
        }
        if (ipAddress.isEmpty())
        {
            r_tmr->start(10000);
            return;
        }
//
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
    if(sock->write(loginQuery) < 0)  //ошибка при записи
    {
        r_tmr->start(5000);
    }
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
    r_tmr->start(5000);
}

void HnaClient::logout()
{
    sock->write("~disconnect;");
    sock->disconnectFromHost();
}

void HnaClient::sockReadyRead()
{
    sock->readAll();
}

void HnaClient::sockSslErrors(QList<QSslError> le)
{
    if (le.size() == 1 && le[0].error() == QSslError::HostNameMismatch)
    {
       ((QSslSocket*)sender())->ignoreSslErrors();
    }
}
