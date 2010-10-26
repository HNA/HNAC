#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(320, 280);
    control = new QtServiceController("Hna Client");
    settings = new QSettings(QSettings::SystemScope, "Hostel2", "hnac");
    login = settings->value("login", "").toString();
    pass = settings->value("password", "").toString();
    ui->lineEdit_login->setText(login);
    ui->lineEdit_password->setText(pass);
    ui->checkBox_showpass->setChecked(false);
    ui->pushButton_loginsave->setVisible(false);
    ui->pushButton_passsave->setVisible(false);
    if(control->isInstalled())
    {
        ui->pushButton_install->setVisible(false);
        ui->pushButton_uninstall->setVisible(true);
        if(control->isRunning())
        {
            ui->pushButton_start->setVisible(false);
            ui->pushButton_restart->setVisible(true);
            ui->pushButton_stop->setVisible(true);
        }
        else
        {
            ui->pushButton_start->setVisible(true);
            ui->pushButton_restart->setVisible(false);
            ui->pushButton_stop->setVisible(false);
        }
    }
    else
    {
        ui->pushButton_install->setEnabled(true);
        ui->pushButton_uninstall->setVisible(false);
        ui->pushButton_start->setVisible(false);
        ui->pushButton_restart->setVisible(false);
        ui->pushButton_stop->setVisible(false);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_start_clicked()
{
    control->start();
    ui->pushButton_start->setVisible(false);
    ui->pushButton_restart->setVisible(true);
    ui->pushButton_stop->setVisible(true);
}

void MainWindow::on_pushButton_stop_clicked()
{
    control->stop();
    ui->pushButton_start->setVisible(true);
    ui->pushButton_stop->setVisible(false);
    ui->pushButton_restart->setVisible(false);
}

void MainWindow::on_pushButton_restart_clicked()
{
    control->stop();
    control->start();
}

void MainWindow::on_pushButton_cancel_clicked()
{
   close();
}

void MainWindow::on_pushButton_install_clicked()
{
    if(QtServiceController::install(qApp->applicationDirPath() + "\\"  + "hnac.exe"))
        {
            ui->pushButton_start->setVisible(true);
            ui->pushButton_install->setVisible(false);
            ui->pushButton_uninstall->setVisible(true);
        }
    else
        {
            QMessageBox::critical(this, "Hna Client Error", "An error occurred during installation");
        }
}

void MainWindow::on_pushButton_uninstall_clicked()
{
    control->uninstall();
    ui->pushButton_install->setVisible(true);
    ui->pushButton_uninstall->setVisible(false);
    ui->pushButton_start->setVisible(false);
    ui->pushButton_stop->setVisible(false);
    ui->pushButton_restart->setVisible(false);
}

void MainWindow::on_lineEdit_login_textChanged(QString text)
{
    if(text != login)
    {
          ui->pushButton_loginsave->setVisible(true);
    }
    else
        {
            ui->pushButton_loginsave->setVisible(false);
        }
}

void MainWindow::on_lineEdit_password_textChanged(QString text)
{
    if(text != pass)
    {
        ui->pushButton_passsave->setVisible(true);
    }
    else
        {
            ui->pushButton_passsave->setVisible(false);
        }
}

void MainWindow::on_checkBox_showpass_clicked()
{
    if(ui->checkBox_showpass->isChecked())
    {
        ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    }
}

void MainWindow::on_pushButton_loginsave_clicked()
{
    login = ui->lineEdit_login->text();
    settings->setValue("login", login);
    ui->pushButton_loginsave->setVisible(false);
}


void MainWindow::on_pushButton_passsave_clicked()
{
    pass = ui->lineEdit_password->text();
    settings->setValue("password", pass);
    ui->pushButton_passsave->setVisible(false);
}
