#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(320, 280);
    control = new QtServiceController("Hna Client");
    settings = new QSettings(QSettings::SystemScope, "Hostel2", "hnac");
    ui->lineEdit_login->setText(settings->value("login", "").toString());
    ui->lineEdit_password->setText(settings->value("password", "").toString());
    ui->checkBox_showpass->setChecked(false);
    if(control->isInstalled())
    {
        ui->pushButton_install->setEnabled(false);
        ui->pushButton_uninstall->setEnabled(true);
        if(control->isRunning())
        {
            ui->pushButton_start->setEnabled(false);
            ui->pushButton_restart->setEnabled(true);
            ui->pushButton_stop->setEnabled(true);
        }
        else
        {
            ui->pushButton_start->setEnabled(false);
            ui->pushButton_restart->setEnabled(true);
            ui->pushButton_stop->setEnabled(true);
        }
    }
    else
    {
        ui->pushButton_install->setEnabled(true);
        ui->pushButton_uninstall->setEnabled(false);
        ui->pushButton_start->setEnabled(false);
        ui->pushButton_restart->setEnabled(false);
        ui->pushButton_stop->setEnabled(false);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_start_clicked()
{
    control->start();
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_restart->setEnabled(true);
    ui->pushButton_stop->setEnabled(true);
}

void MainWindow::on_pushButton_stop_clicked()
{
    control->stop();
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_stop->setEnabled(false);
    ui->pushButton_restart->setEnabled(false);
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

void MainWindow::on_pushButton_saveclose_clicked()
{
    if(ui->pushButton_saveclose->text() == tr("Close"))
    {
        close();
    }
    else
    {
        settings->setValue("login", ui->lineEdit_login->text());
        settings->setValue("password", pass);
        ui->pushButton_saveclose->setText(tr("Close"));
    }
}

void MainWindow::on_pushButton_install_clicked()
{
    QtServiceController::install(qApp->applicationDirPath() + "\\"  + "hnac.exe");
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_install->setEnabled(false);
    ui->pushButton_uninstall->setEnabled(true);
}

void MainWindow::on_pushButton_uninstall_clicked()
{
    control->uninstall();
    ui->pushButton_install->setEnabled(true);
    ui->pushButton_uninstall->setEnabled(false);
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_stop->setEnabled(false);
    ui->pushButton_restart->setEnabled(false);
}

void MainWindow::on_lineEdit_login_textChanged(QString text)
{
    login = text;
    if(text != settings->value("login", "").toString())
    {
        ui->pushButton_saveclose->setText(tr("Save"));
    }
}

void MainWindow::on_lineEdit_password_textChanged(QString text)
{
    pass = text;
    if(text != settings->value("password", "").toString())
    {
        ui->pushButton_saveclose->setText(tr("Save"));
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
