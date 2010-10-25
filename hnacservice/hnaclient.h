#ifndef HNACLIENT_H
#define HNACLIENT_H

#include <QAbstractSocket>
#include <QSslError>
#include <QSettings>
#include <QTimer>

class QSslSocket;
class QSslCertificate;
class QSettings;
class QTimer;

class HnaClient : public QObject
{
    Q_OBJECT
public:
    explicit HnaClient(QObject *parent = 0);
    ~HnaClient();

public slots:
    void logout();
    void tryLogin();
<<<<<<< HEAD:hnacservice/hnaclient.h
#if !defined(Q_WS_WIN)
    void tryLogin(QString login, QString pass);
#endif
=======
    void tryLogin(QString login, QString pass);
>>>>>>> 973accdc68e3497f1ea66c5f766c55227a1bd9b7:hnaclient.h

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
    bool newData;
};

#endif // HNACLIENT_H
