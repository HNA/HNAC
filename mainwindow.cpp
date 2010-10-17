#include "mainwindow.h"
#include "login.h"
#include "logout.h"
#include "hnaclient.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    settings = new QSettings("Hostel2", "hnac");
    qApp->installTranslator(&translator);
    translator.load(":/translations/hnac_" + settings->value("language", "en").toString());
    setupGui();
    retranslateUI();
}

void MainWindow::setupGui()
{
    QWidget *widget = new QWidget(this);
    mainLayout = new QStackedLayout(widget);
    li = new login(widget);
    lo = new logout(widget);
    mainLayout->addWidget(li);
    mainLayout->addWidget(lo);
    mainLayout->setCurrentIndex(0);
    c = new HnaClient(widget);
    connect(li, SIGNAL(tryLogin(QString,QString)), c, SLOT(tryLogin(QString,QString)));
    connect(lo, SIGNAL(logoutClk()), c, SLOT(logout()));
    connect(c, SIGNAL(stateChanged()), this, SLOT(stateChanged()));
    connect(c, SIGNAL(setIP(QString)), lo, SLOT(setIP(QString)));
    widget->setLayout(mainLayout);
    setCentralWidget(widget);
    setFixedSize(200, 100);
    move(settings->value("pos", QApplication::desktop()->availableGeometry().center()).toPoint());
    Qt::WindowFlags wflags;
    wflags = Qt::FramelessWindowHint;
    setWindowFlags(wflags);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    createTrayIcon();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/offline.png"));
    setWindowIcon(QIcon(":/icons/hnac.png"));

    isSavePassword = new QAction(this);
    isSavePassword->setCheckable(true);
    isSavePassword->setChecked(settings->value("isSave", false).toBool());
    connect(isSavePassword, SIGNAL(toggled(bool)), this, SLOT(setIsSave(bool)));

    isHideOnClose = new QAction(this);
    isHideOnClose->setCheckable(true);
    isHideOnClose->setChecked(settings->value("isHide", false).toBool());
    connect(isHideOnClose, SIGNAL(toggled(bool)), this, SLOT(setIsHide(bool)));

    autorunAction = new QAction(this);
    autorunAction->setCheckable(true);
    autorunAction->setChecked(settings->value("autorun", false).toBool());
    connect(autorunAction, SIGNAL(toggled(bool)), this, SLOT(setAutorun(bool)));

    showHideAction = new QAction(this);
    connect(showHideAction, SIGNAL(triggered()), this, SLOT(showHide()));

    quitAction = new QAction(this);
    quitAction->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(Exit()));

    aboutAction = new QAction(this);
    aboutAction->setIcon(style()->standardIcon(QStyle::SP_FileDialogInfoView));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    opacityMenu = new QMenu(this);
    QActionGroup *actsOpacity = new QActionGroup(this);
    for(int i = 1; i <= 10; i++)
    {
        QAction *action = new QAction(actsOpacity);
        action->setText(QString::number(i * 10) + "%");
        action->setData(i);
        action->setCheckable(true);
        if(settings->value("opacity", 10).toInt() == i)
            action->setChecked(true);
        actsOpacity->addAction(action);
        opacityMenu->addAction(action);
    }
    connect(actsOpacity, SIGNAL(triggered(QAction*)), this, SLOT(setOpacity(QAction*)));

    QAction *rusLanguageAction = new QAction(tr("Russian"), this);
    rusLanguageAction->setCheckable(true);
    rusLanguageAction->setData("ru");

    QAction *engLanguageAction = new QAction(tr("English"), this);
    engLanguageAction->setCheckable(true);
    engLanguageAction->setData("en");

    languageMenu = new QMenu(this);
    languageMenu->addAction(rusLanguageAction);
    languageMenu->addAction(engLanguageAction);

    QActionGroup *languageGroup = new QActionGroup(this);
    languageGroup->addAction(rusLanguageAction);
    languageGroup->addAction(engLanguageAction);

    if(settings->value("language", "en").toString() == "en")
        {
            engLanguageAction->setChecked(true);
        }
    else
        {
            rusLanguageAction->setChecked(true);
        }
    connect(languageGroup, SIGNAL(triggered(QAction *)), this, SLOT(setLanguage(QAction *)));
    QMenu *trayIconMenu = new QMenu(this);
    optionsTrayMenu = new QMenu(trayIconMenu);
    optionsTrayMenu->setIcon(QIcon(":/icons/settings.png"));
    trayIconMenu->addAction(showHideAction);
    trayIconMenu->addMenu(optionsTrayMenu);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);
    optionsTrayMenu->addAction(isSavePassword);
    optionsTrayMenu->addAction(isHideOnClose);
    optionsTrayMenu->addAction(autorunAction);
    optionsTrayMenu->addMenu(languageMenu);
    optionsTrayMenu->addMenu(opacityMenu);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();
}

void MainWindow::retranslateUI()
{
    setWindowTitle(tr("HNA Client"));
    isSavePassword->setText(tr("Save password"));
    isHideOnClose->setText(tr("Hide on close"));
    autorunAction->setText(tr("Run at startup"));
    quitAction->setText(tr("Quit"));
    aboutAction->setText(tr("About HNA Client..."));
    opacityMenu->setTitle(tr("Opacity"));
    optionsTrayMenu->setTitle(tr("Options"));
    languageMenu->setTitle(tr("Language"));
    if(isHidden())
        {
            showHideAction->setText(tr("Show window"));
        }
    else
        {
            showHideAction->setText(tr("Hide window"));
        }
    li->retranslateUI();
    lo->retranslateUI();
}

void MainWindow::setLanguage(QAction *action)
{
    QString locale = action->data().toString();
    translator.load(":/translations/hnac_"+locale);
    settings->setValue("language", locale);
    retranslateUI();
}

void MainWindow::setAutorun(bool state)
{
    settings->setValue("autorun", state);
    QSettings *autorun;
    if(state)
        {
#ifdef Q_WS_WIN
            autorun = new QSettings("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
            autorun->setValue("hna client", qApp->applicationFilePath());
#endif
        }
    else
        {
#ifdef Q_WS_WIN
            autorun = new QSettings("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
            autorun->remove("hna client");
#endif
        }
}

void MainWindow::stateChanged()
{
    switch(c->state())
    {
    case HnaClient::Disconnected:
        ((login*)mainLayout->widget(0))->unlockEdit();
        mainLayout->setCurrentIndex(0);
        trayIcon->setIcon(QIcon(":/icons/offline.png"));
        break;
    case HnaClient::Connected:
        mainLayout->setCurrentIndex(1);
        trayIcon->setIcon(QIcon(":/icons/online.png"));
        break;
    default:
        break;
    }
}


void MainWindow::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
    {
        setHidden(!isHidden());
    }
}

void MainWindow::showHide()
{
    setHidden(!isHidden());
}

void MainWindow::setIsSave(bool state)
{
    settings->setValue("isSave", state);
}

void MainWindow::setIsHide(bool state)
{
    settings->setValue("isHide", state);
}

void MainWindow::hideEvent(QHideEvent *event)
{
    if(event->type() == QEvent::Hide)
    {
        showHideAction->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton));
        showHideAction->setText(tr("Show window"));
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    if(event->type() == QEvent::Show)
    {
        showHideAction->setIcon(style()->standardIcon(QStyle::SP_TitleBarMinButton));
        showHideAction->setText(tr("Hide window"));
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setOpacity(settings->value("opacity", 10).toReal() / 10);
    painter.drawImage(rect(), QImage(":/images/background.png"));
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        move(pos() + (event->pos()-startPos));
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        startPos = event->pos();
    }
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    settings->setValue("pos", event->pos());
}

void MainWindow::setOpacity(QAction *action)
{
    settings->setValue("opacity", action->data().toInt());
    repaint();
}

void MainWindow::about()
{
    QMessageBox::about(this, "HNAc v2.0",
                       "<p><b>HNA Client 2.0</b></p>"
                          "<p>Created by Admin</p>"
                          "<p>Recreated by Gangs</p>"
                          "<b>icq:</b> 359094611");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(settings->value("isHide", false).toBool())
    {
        event->ignore();
        hide();
    }
    else
    {
        exit(0);
    }
}

void MainWindow::Exit()
{
    exit(0);
}
