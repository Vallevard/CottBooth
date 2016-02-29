#include "startscreen.h"
#include "ui_startscreen.h"
#include "mainwindow.h"
#include <QDebug>

StartScreen::StartScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartScreen)
{
    ui->setupUi(this);    
    ui->btnStartSession->setFont(MainWindow::instance()->m_pAwesome->font(72));
    ui->btnSettings->setFont(MainWindow::instance()->m_pAwesome->font(32));
    ui->btnRespawnSession->setFont(ui->btnSettings->font());
    ui->btnInfo->setFont(ui->btnSettings->font());

    ui->btnSettings->setText( QString( fa::cogs ) );
    ui->btnStartSession->setText( QString( fa::cameraretro ) );
    ui->btnRespawnSession->setText( QString( fa::retweet ) );
    ui->btnInfo->setText( QString( fa::infocircle ) );


    this->connect(this->ui->btnSettings, &QToolButton::clicked, this, &StartScreen::btnSettingsClicked);
}



StartScreen::~StartScreen()
{
    delete ui;
}

void StartScreen::btnSettingsClicked()
{
    MainWindow::instance()->askForMasterPassword();
}
