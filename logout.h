#ifndef LOGOUT_H
#define LOGOUT_H

#include <QtGui>

class logout : public QWidget {
    Q_OBJECT
public:
    logout(QWidget *parent = 0);
    ~logout();
    void retranslateUI();

private:
    QPushButton *btnDisconnect;
    QListWidget *listInfo;
    void setupGui();

private slots:
    void on_pushButton_clicked();

public slots:
    void setIP(QString);

signals:
    void logoutClk();
};

#endif // LOGOUT_H
