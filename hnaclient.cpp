#include "hnaclient.h"
#include "login.h"

#include <QSslSocket>
#include <QHostAddress>

HnaClient::HnaClient(QObject *parent) :
    QObject(parent), m_state(Disconnected), loggedIn(false)
{
    settings = new QSettings("Hostel2", "hnac");
    sock = new QSslSocket(this);

    expectedSslErrors.append(QSslError(QSslError::HostNameMismatch));

    connect(sock, SIGNAL(connected()), this, SLOT(sockConnected()));
    connect(sock, SIGNAL(disconnected()), this, SLOT(sockDisconnected()));
    connect(sock, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(sockError(QAbstractSocket::SocketError)));
    connect(sock, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(sockStateChanged(QAbstractSocket::SocketState)));
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
}

HnaClient::~HnaClient()
{
    delete sock;
}

void HnaClient::sockSendPing()
{
    sock->write("~ping;");
}

HnaClient::HnaState HnaClient::state()
{
    return m_state;
}

void HnaClient::tryLogin(QString login, QString pass)
{
    sock->connectToHostEncrypted("172.30.0.1", 3816, QIODevice::ReadWrite);
    m_login = login;
    m_pass = pass;
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
            //maybe not work with internet connection !!!!
            .append(";");
    emit setIP(sock->localAddress().toString());
    m_state = Connected;
    sock->write(loginQuery);
}

void HnaClient::sockConnected()
{
    if(settings->value("isSave", true).toBool())
    {
        settings->setValue("login", m_login);
        settings->setValue("password", m_pass);
    }
}

void HnaClient::sockDisconnected()
{
    m_state = Disconnected;
}

void HnaClient::sockError(QAbstractSocket::SocketError)
{
    QMessageBox::critical((QWidget*)parent(), QString("HNAClient"),
                          QString("Socket error: ") + sock->errorString());
}

void HnaClient::sockStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        m_state = Disconnected;
        emit stateChanged();
    }
    else if (socketState == QAbstractSocket::ConnectedState)
    {
        m_state = AboutToConnect;
        emit stateChanged();
    }
}

void HnaClient::logout()
{
    sock->write("~disconnect;");
    sock->disconnectFromHost();
}

void HnaClient::sockReadyRead()
{
    QByteArray data = sock->readAll();
    QStringList responseList = QString::fromUtf8(data.data(),
                                                 data.size()).split(";");
    for (int i = 0; i < responseList.size(); i++)
    {
        if (responseList[i] == "~logged_in")
        {
            m_state = Connected;
            emit stateChanged();
        }
        else
        {

        }
    }
}

void HnaClient::sockSslErrors(QList<QSslError> le)
{
    if (le.size() == 1 && le[0].error() == QSslError::HostNameMismatch)
    {
       ((QSslSocket*)sender())->ignoreSslErrors();
    }
}
