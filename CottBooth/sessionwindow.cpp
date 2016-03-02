#include "globals.h"
#include "sessionwindow.h"
#include "ui_sessionwindow.h"
#include "mainwindow.h"
#include "settings.h"
#include "cameracontroller.h"
#include "sessionmanager.h"
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QtConcurrent>


#ifdef Q_OS_MAC
    #include <QProcess>
#endif

SessionWindow::SessionWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SessionWindow)
{
    ui->setupUi(this);
    m_pSession = nullptr;
    m_pImageList = new QList<QObject>();

    ui->btnBack->setFont(MainWindow::instance()->m_pAwesome->font(32));
    ui->btnBack->setText( QString( fa::backward ) );
    connect(ui->btnBack, &QToolButton::clicked, [=]() {
        if(validateCredentials()) {
            MainWindow::instance()->closeSessionWindow();
            delete m_pSession;
            m_pSession = nullptr;
        }
        else
        {
            QMessageBox alert(QMessageBox::Critical,"Error", "You entered the wrong credentials", QMessageBox::Ok, this, DIALOG_MASK);
            alert.exec();
        }
    });

    CameraController *c = CameraController::instance();   

    connect(c, &CameraController::ready, this, [=]() {
        qDebug() << "Camera ready!";
        c->setCopyToPath(m_pSession->savePath());
    });

    connect(c, &CameraController::fail, [=](int, QString message){
        qDebug() << "Camera error: " << message;
    });

    connect(c, &CameraController::captureComplete, [=](QString path){
        qDebug() << "File Created: " << path;
        m_pSession->incrementImages();        
    });    
}

SessionWindow::~SessionWindow()
{
    delete ui;
}

void SessionWindow::initCamera()
{
    CameraController *c = CameraController::instance();
    c->init();
}

bool SessionWindow::openSession(QString credentials) {
    if (credentials == nullptr) {
        int ret = showCredentialDialog();

        if(!ret)
            return false;
    }
    else
    {
        Session *s = SessionManager::respawnSession(credentials);
        if(s == nullptr)
            return false;

        m_pSession = s;
    }

    this->initCamera();
    ui->lblTitle->setText(m_pSession->sessionName());

    return true;
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

    m_pSession = SessionManager::createSession(sessionName, path);
    QString credentials = "<br /><br /><b style=\"color: green\">" + m_pSession->credentials() + "</b>";
    QMessageBox alert(QMessageBox::Information, tr("Session credentials"),
                             "<qt>" + tr("Please remember the following credentials. You need these e.g. to respawn this session.") + credentials + "</qt>",
                             QMessageBox::Ok, this, DIALOG_MASK);
    alert.exec();

    m->toggleBackgroundMode(false, 0);

    return true;
}

bool SessionWindow::validateCredentials()
{
    bool ok;
    QString credentials = QInputDialog::getText(this, tr("Credentials"),
                tr("Enter session credentials to perform the selected action."), QLineEdit::Normal,
                NULL, &ok, DIALOG_MASK);
    bool access = (m_pSession->credentials().compare(credentials) == 0);
    return (ok && access);
}

void SessionWindow::showEvent(QShowEvent * /*event*/)
{
    loadImages();
    //QtConcurrent::run(this, &SessionWindow::loadImages);
}

void SessionWindow::hideEvent(QHideEvent * /* event */)
{
    qDeleteAll(ui->imageContainer->findChildren<QWidget *>());
    CameraController::instance()->uninit();
}

void SessionWindow::loadImages()
{
    int w = ui->imageContainer->width() / 6;
    qDebug() << "width:" << w << " org " << ui->imageContainer->width();
    for(int x = 0; x < 5; x++)
    {
        for(int y = 0; y < 500; y++)
        {
            QGridLayout *grid = qobject_cast<QGridLayout *>(ui->imageContainer->layout());
            int w = ui->imageContainer->width() / 6;
            QLabel *lblTest = new QLabel(ui->imageContainer);
            lblTest->setStyleSheet("border: 1px solid green;");
            lblTest->setFixedSize(w,w);
            lblTest->setScaledContents(true);
            //lblTest->setPixmap(pixMap);
            grid->addWidget(lblTest, y, x, Qt::AlignCenter);

            /*QImage img("/Users/tobi/Pictures/12545224383_6a0cc0991e_z.jpg");
            QPixmap map = QPixmap::fromImage(img);
            emit imageLoaded(map, x, y);*/
        }
    }
}

void SessionWindow::imageLoaded(QPixmap pixMap, int x, int y)
{

}
