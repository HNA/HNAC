#ifndef HNACLIENT_H
#define HNACLIENT_H

#include <QAbstractSocket>
#include <QSslError>
#include <QSettings>
#include <QTimer>

class QSslSocket;
class QSslCertificate;

class HnaClient : public QObject
{
    Q_OBJECT
public:
    explicit HnaClient(QObject *parent = 0);
    ~HnaClient();

public slots:
    void logout();
    void tryLogin();
    void tryLogin(QString login, QString pass);

private slots:
    void sockConnected();
    void sockError(QAbstractSocket::SocketError);
    void sockReadyRead();
    void sockEncrypted();
    void sockSslErrors(QList<QSslError>);
    void sockSendPing();

private:
    QSslSocket *sock;
    QByteArray loginQuery;
    QSettings *settings;

    QList<QSslCertificate> certsCA;
    QList<QSslError> expectedSslErrors;

    QString m_login;
    QString m_pass;
    QTimer *m_tmr;
    QTimer *r_tmr;
};

#endif // HNACLIENT_H
