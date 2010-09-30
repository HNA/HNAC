#ifndef HNACLIENT_H
#define HNACLIENT_H

#include <QtGui>
#include <QAbstractSocket>
#include <QSslError>

class QSslSocket;
class QSslCertificate;

class HnaClient : public QObject
{
Q_OBJECT
public:
    explicit HnaClient(QObject *parent = 0);
    ~HnaClient();

    enum HnaState {Disconnected, Connected, AboutToConnect};
    HnaState state();
signals:
    void stateChanged();
    void setIP(QString);
private slots:
    void tryLogin(QString login, QString pass);
    void logout();
    void sockConnected();
    void sockDisconnected ();
    void sockError(QAbstractSocket::SocketError);
    void sockStateChanged ( QAbstractSocket::SocketState socketState );
    void sockReadyRead();

    void sockEncrypted();
    void sockSslErrors(QList<QSslError>);

    void sockSendPing();

private:
    QSslSocket *sock;
    HnaState m_state;
    QByteArray loginQuery;
    bool loggedIn;
    QSettings *settings;

    QList<QSslCertificate> certsCA;
    QList<QSslError> expectedSslErrors;

    QString m_login;
    QString m_pass;
    QTimer *m_tmr;
};

#endif // HNACLIENT_H
