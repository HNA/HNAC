#ifndef LOGIN_H
#define LOGIN_H

#include <QtGui>

class login : public QWidget
{
    Q_OBJECT
public:
    login(QWidget *parent = 0);
    ~login();
    void retranslateUI();

public slots:
    void lockEdit();
    void unlockEdit();

private:
    QString m_login;
    QString m_password;
    QSettings *settings;
    QLabel *label_login;
    QLabel *label_pass;
    QPushButton *button;
    void setupGui();

signals:
    void tryLogin(QString login, QString password);

private slots:
    void on_btnLogin_clicked();
    void setLogin(QString);
    void setPassword(QString);
    void setIsSave(int);
};

#endif // LOGIN_H
