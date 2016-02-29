#include "globals.h"
#include "sessionwindow.h"
#include "ui_sessionwindow.h"
#include "mainwindow.h"
#include "settings.h"
#include "cameracontroller.h"
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

SessionWindow::SessionWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SessionWindow)
{
    ui->setupUi(this);
    m_pSession = nullptr;

    ui->btnBack->setFont(MainWindow::instance()->m_pAwesome->font(32));
    ui->btnBack->setText( QString( fa::backward ) );
    connect(ui->btnBack, &QToolButton::clicked, [=]() {
        MainWindow::instance()->closeSessionWindow();
        m_pSession = nullptr;
    });
}

SessionWindow::~SessionWindow()
{
    delete ui;
}

void SessionWindow::initCamera()
{
    CameraController *c = CameraController::instance();
    connect(c, &CameraController::ready, this, [=]() {
        qDebug() << "Camera ready!";
    });

    connect(c, &CameraController::initializationFailed, [=](int, QString message){
        qDebug() << " cool: " << message;
    });

    connect(c, &CameraController::imageCreated, [=](CameraFilePath path){
        qDebug() << "File Created: " << path.folder << path.name;
    });

    c->init();
}

bool SessionWindow::openSession(QString sid) {
    if (sid == NULL) {
        int ret = showCredentialDialog();

        if(ret)
        {
            this->initCamera();
        }

        return ret;
    }

    return false;
}

bool SessionWindow::showCredentialDialog()
{
    MainWindow *m = MainWindow::instance();
    m->toggleBackgroundMode(true, 0);
    bool ok;
    QString sessionName = QInputDialog::getText(this, tr("Session Name"),
                tr("Enter a name for this session (e.g. Jon\'s 29th birthday)"), QLineEdit::Normal,
                NULL, &ok, DIALOG_MASK);

    if (!ok || sessionName.isEmpty())
    {
        m->closeSessionWindow();
        m->toggleBackgroundMode(false, 0);
        return false;
    }

    QString path;
    if(Settings::instance()->boolValue(Settings::ASK_FOR_SAVE_PATH) == true)
    {
        // TODO: show folder browse dialog
    } else {
        path = Settings::instance()->stringValue(Settings::SAVE_PATH);
    }

    m_pSession = new Session(sessionName, path);
    QMessageBox alert(QMessageBox::Information, tr("Session credentials"),
                             tr("Please remember the following credentials. You need these e.g. to respawn this session. \n\n") + m_pSession->credentials(),
                             QMessageBox::Ok, this, DIALOG_MASK);
    alert.exec();

    m->toggleBackgroundMode(false, 0);

    return true;
}
