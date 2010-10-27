#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QMessageBox>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    settings = new QSettings(QSettings::SystemScope, "Hostel2", "hnac");
    qApp->installTranslator(&translator);
    translator.load(":/translation/HnacConfig_" + settings->value("language", "en").toString());
    ui->setupUi(this);
    setFixedSize(320, 280);
    control = new QtServiceController("Hna Client");
    settings = new QSettings(QSettings::SystemScope, "Hostel2", "hnac");
    login = settings->value("login", "").toString();
    pass = settings->value("password", "").toString();

    ui->lineEdit_login->setText(login);
    ui->lineEdit_password->setText(pass);
    Qt::WindowFlags wflags;
    wflags = Qt::Dialog;
    setWindowFlags(wflags);
    ui->checkBox_showpass->setChecked(false);
    ui->pushButton_loginsave->setVisible(false);
    ui->pushButton_passsave->setVisible(false);
    createLanguageMenu();
    if(control->isInstalled())
    {
        ui->statusBar->showMessage(tr("Hnac service already instaled. Press \"Start\""));
        ui->pushButton_install->setVisible(false);
        ui->pushButton_uninstall->setVisible(true);
        if(control->isRunning())
        {
            ui->statusBar->showMessage(tr("Hnac service is running"));
            ui->pushButton_start->setVisible(false);
            ui->pushButton_restart->setVisible(true);
            ui->pushButton_stop->setVisible(true);
            ui->pushButton_uninstall->setVisible(false);
        }
        else
        {
            ui->pushButton_start->setVisible(true);
            ui->pushButton_restart->setVisible(false);
            ui->pushButton_stop->setVisible(false);
            ui->pushButton_uninstall->setVisible(true);
        }
    }
    else
    {
        ui->statusBar->showMessage(tr("Press \"Install\" to install service"));
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

void MainWindow::createLanguageMenu()
{
    languageBox = new QComboBox(this);
    languageBox->addItem("Russian", "ru");
    languageBox->addItem("English", "en");
    if(settings->value("language", "en").toString() == "en")
    {
        languageBox->setCurrentIndex(1);
    }
    else
    {
        languageBox->setCurrentIndex(0);
    }
    connect(languageBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setLanguage(int)));
    ui->statusBar->addPermanentWidget(languageBox);
}

void MainWindow::setLanguage(int index)
{
    settings->setValue("language", languageBox->itemData(index));
}

void MainWindow::on_pushButton_start_clicked()
{
    if(control->start())
    {
        ui->statusBar->showMessage(tr("Hnac service is running"));
        ui->pushButton_start->setVisible(false);
        ui->pushButton_restart->setVisible(true);
        ui->pushButton_stop->setVisible(true);
        ui->pushButton_uninstall->setVisible(false);
    }
    else
    {
        ui->statusBar->showMessage(tr("An error occurred during starting"));
    }
}

void MainWindow::on_pushButton_stop_clicked()
{
    if(control->stop())
    {
        ui->statusBar->showMessage(tr("Hnac service is stopped"));
        ui->pushButton_start->setVisible(true);
        ui->pushButton_stop->setVisible(false);
        ui->pushButton_restart->setVisible(false);
        ui->pushButton_uninstall->setVisible(true);
    }
    else
    {
        ui->statusBar->showMessage(tr("An error occurred during stoping"));
    }
}

void MainWindow::on_pushButton_restart_clicked()
{
    if(control->stop())
    {
        ui->statusBar->showMessage(tr("Hnac service is stopped"));
        ui->pushButton_start->setVisible(true);
        ui->pushButton_stop->setVisible(false);
        ui->pushButton_restart->setVisible(false);
        ui->pushButton_uninstall->setVisible(true);
        if(control->start())
        {
            ui->statusBar->showMessage(tr("Hnac service is running"));
            ui->pushButton_start->setVisible(false);
            ui->pushButton_restart->setVisible(true);
            ui->pushButton_stop->setVisible(true);
            ui->pushButton_uninstall->setVisible(false);
        }
    }
}

void MainWindow::on_pushButton_cancel_clicked()
{
    close();
}

void MainWindow::on_pushButton_install_clicked()
{
    if(QtServiceController::install(qApp->applicationDirPath() + "\\"  + "hnac.exe"))
    {
        ui->statusBar->showMessage(tr("Hnac service is instaled. Press \"Start\""));
        ui->pushButton_start->setVisible(true);
        ui->pushButton_install->setVisible(false);
        ui->pushButton_uninstall->setVisible(true);
    }
    else
    {
        ui->statusBar->showMessage(tr("An error occurred during installation"));
    }
}

void MainWindow::on_pushButton_uninstall_clicked()
{
    if(control->uninstall())
    {
        ui->statusBar->showMessage(tr("Hnac service is uninstaled"));
        ui->pushButton_install->setVisible(true);
        ui->pushButton_uninstall->setVisible(false);
        ui->pushButton_start->setVisible(false);
        ui->pushButton_stop->setVisible(false);
        ui->pushButton_restart->setVisible(false);
    }
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
    ui->statusBar->showMessage(tr("Login saved"));
    ui->pushButton_loginsave->setVisible(false);
}


void MainWindow::on_pushButton_passsave_clicked()
{
    pass = ui->lineEdit_password->text();
    settings->setValue("password", pass);
    ui->statusBar->showMessage(tr("Password saved"));
    ui->pushButton_passsave->setVisible(false);
}
