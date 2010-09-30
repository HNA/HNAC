#include "login.h"

login::login(QWidget *parent) :
    QWidget(parent)
{
    settings = new QSettings("Hostel2", "hnac");
    setupGui();
}

login::~login()
{
}

void login::setupGui()
{
    QGridLayout *g_layout = new QGridLayout(this);
    label_login = new QLabel(this);
    label_login->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    label_pass = new QLabel(this);
    label_pass->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLineEdit *lineLogin = new QLineEdit(this);
    QLineEdit *linePass = new QLineEdit(this);
    linePass->setEchoMode(QLineEdit::Password);
    connect(lineLogin, SIGNAL(textChanged(QString)), this, SLOT(setLogin(QString)));
    connect(linePass, SIGNAL(textChanged(QString)), this, SLOT(setPassword(QString)));
    button = new QPushButton(this);
    button->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    linePass->setText(settings->value("password", "").toString());
    lineLogin->setText(settings->value("login", "").toString());
    connect(button, SIGNAL(clicked()), this, SLOT(on_btnLogin_clicked()));
    g_layout->addWidget(label_login, 0, 0, 1, 1);
    g_layout->addWidget(label_pass, 1, 0, 1, 1);
    g_layout->addWidget(lineLogin, 0, 1, 1, 2);
    g_layout->addWidget(linePass, 1, 1, 1, 2);
    g_layout->addWidget(button, 2, 2, 1, 1);
    setLayout(g_layout);
}

void login::setLogin(QString text)
{
    m_login = text;
}

void login::setPassword(QString pass)
{
    m_password = pass;
}

void login::setIsSave(int state)
{
    settings->setValue("isSave", state == 2);
}

void login::on_btnLogin_clicked()
{
    lockEdit();
    emit tryLogin(m_login, m_password);
}

void login::lockEdit()
{
    setEnabled(false);
}

void login::unlockEdit()
{
    setEnabled(true);
}

void login::retranslateUI()
{
       label_login->setText("<font color=#F2D52E>" + tr("Login:") + "</font>");
       label_pass->setText("<font color=#F2D52E>" + tr("Password:") + "</font>");
       button->setText(tr("Connect"));
}
