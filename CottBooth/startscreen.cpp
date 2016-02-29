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


    connect(ui->btnSettings, &QToolButton::clicked, [=](){
        MainWindow::instance()->askForMasterPassword();
    });

    connect(ui->btnStartSession, &QToolButton::clicked, [=](){
        MainWindow::instance()->openSessionWindow();
    });
}



StartScreen::~StartScreen()
{
    delete ui;
}
