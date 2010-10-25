#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qtservice.h"

class QSettings;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QtServiceController *control;
    QString login;
    QString pass;
    QSettings *settings;

private slots:
    void on_checkBox_showpass_clicked();
    void on_lineEdit_password_textChanged(QString );
    void on_lineEdit_login_textChanged(QString );
    void on_pushButton_uninstall_clicked();
    void on_pushButton_install_clicked();
    void on_pushButton_saveclose_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_restart_clicked();
    void on_pushButton_stop_clicked();
    void on_pushButton_start_clicked();
};

#endif // MAINWINDOW_H
