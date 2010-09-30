#include "logout.h"

logout::logout(QWidget *parent) :
    QWidget(parent)
{
    setupGui();
}

logout::~logout()
{
}

void logout::setupGui()
{
    QVBoxLayout *v_layout = new QVBoxLayout(this);
    btnDisconnect = new QPushButton(this);
    btnDisconnect->setIcon(style()->standardIcon(QStyle::SP_DialogResetButton));
    connect(btnDisconnect, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    listInfo = new QListWidget(this);
    v_layout->addWidget(listInfo);
    v_layout->addWidget(btnDisconnect);
    setLayout(v_layout);
}

void logout::on_pushButton_clicked()
{
    emit logoutClk();
}

void logout::retranslateUI()
{
    btnDisconnect->setText(tr("Disconnect"));
}

void logout::setIP(QString ip)
{
    listInfo->addItem(tr("Current IP: ") + ip);
}
