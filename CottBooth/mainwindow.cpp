#include "globals.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startscreen.h"
#include "settingswindow.h"
#include "sessionwindow.h"
#include "settings.h"
#include <QFile>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QGraphicsOpacityEffect>
#include <assert.h>

MainWindow* MainWindow::s_instance = NULL;

MainWindow* MainWindow::instance() {
    return s_instance;
}

MainWindow::MainWindow(QWidget *parent) :    
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_pAwesome = new QtAwesome(qApp);
    m_pAwesome->initFontAwesome();
    m_pAwesome->setDefaultOption("color", QColor(0,0,0));
    m_pAwesome->setDefaultOption("color-selected", QColor(255,255,255));
    assert(!s_instance);
    s_instance = this;

    ui->setupUi(this);
    mergeStyles();

    StartScreen *start = new StartScreen();
    SettingsWindow *settings = new SettingsWindow();
    SessionWindow *session = new SessionWindow();

    ui->stackContainer->addWidget(start);
    ui->stackContainer->addWidget(settings);
    ui->stackContainer->addWidget(session);

    if(Settings::instance()->boolValue(Settings::FIRST_RUN, true))
    {
        this->enterForMasterPassword(*settings);
    }
}

MainWindow::~MainWindow()
{
    delete m_pAwesome;
    delete ui;
}

void MainWindow::openSettingsWindow()
{
    ui->stackContainer->setCurrentIndex(1);
}

void MainWindow::closeSettingsWindow()
{
    ui->stackContainer->setCurrentIndex(0);
}

void MainWindow::openSessionWindow(QString credentials)
{
    SessionWindow *w = qobject_cast<SessionWindow*>(ui->stackContainer->widget(2));
    bool ret = w->openSession(credentials);
    if( ret )
    {
        ui->stackContainer->setCurrentIndex(2);
    }
    else if (ret == false && credentials != nullptr)
    {
        QMessageBox alert(QMessageBox::Information,"Session not found", "No session were found for the entered credentials.", QMessageBox::Ok, this, DIALOG_MASK);
        alert.exec();
    }
}

void MainWindow::closeSessionWindow()
{
    ui->stackContainer->setCurrentIndex(0);
}

void MainWindow::mergeStyles()
{
    QFile baseStyle(":/styles/default.css");
    QByteArray mergedStyles;
    if(baseStyle.open(QFile::ReadOnly|QIODevice::Text))
    {
        mergedStyles.append(baseStyle.readAll());
        baseStyle.close();
    }

    QString theme = Settings::instance()->stringValue(Settings::THEME);
    if(theme.compare("Default") != 0) {
        QFile customStyle(QApplication::applicationDirPath() + "/themes/"+ theme + "/" + theme + ".css");
        if(customStyle.open(QFile::ReadOnly|QIODevice::Text))
        {
            mergedStyles.append(customStyle.readAll());
            customStyle.close();
        }
    }

    this->setStyleSheet(mergedStyles);
}

void MainWindow::askForMasterPassword()
{
    toggleBackgroundMode(true, 0);
    bool ok;
    QString passwd = QInputDialog::getText(this, tr("Master-Password"),
                tr("Enter the valid Master-Password to enter this section:"), QLineEdit::Normal,
                NULL, &ok, DIALOG_MASK);

    if (ok && !passwd.isEmpty())
    {
        QString master = Settings::instance()->stringValue(Settings::MASTER_PASSWORD);
        QString hash = QString(QCryptographicHash::hash(passwd.toUtf8(), QCryptographicHash::Sha256).toHex());
        if(master == hash) {
            openSettingsWindow();
        } else {
            QMessageBox alert(QMessageBox::Critical,"Error", "You enterd a wrong Password", QMessageBox::Ok, this, DIALOG_MASK);
            alert.exec();
        }
    }

    toggleBackgroundMode(false, 0);
}

void MainWindow::enterForMasterPassword(SettingsWindow &settings)
{
    bool ok;
    QString passwd = QInputDialog::getText(this, tr("Master-Password"),
                                        tr("You running the Applicatation the first time, please enter a Master-Password:"), QLineEdit::Normal,
                                        NULL, &ok, DIALOG_MASK);
    if (ok && !passwd.isEmpty())
    {
        settings.saveEncryptedPassword(passwd);
    }
}

void MainWindow::toggleBackgroundMode(bool flag, int index){
    if(!flag)
    {
        ui->stackContainer->setGraphicsEffect(0);
        ui->stackContainer->widget(index)->setGraphicsEffect(0);
    }
    else
    {
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
        effect->setOpacity(0.1);

        QGraphicsBlurEffect *blur = new QGraphicsBlurEffect(this);
        blur->setBlurRadius(8);
        blur->setBlurHints(QGraphicsBlurEffect::QualityHint);

        ui->stackContainer->setGraphicsEffect(effect);
        ui->stackContainer->widget(index)->setGraphicsEffect(blur);
    }
}
