#include "globals.h"
#include "sessionwindow.h"
#include "ui_sessionwindow.h"
#include "mainwindow.h"
#include "settings.h"
#include "cameracontroller.h"
#include "sessionmanager.h"
#include "imageworker.h"
#include "clickablelabel.h"
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QtConcurrent>


#ifdef Q_OS_MAC
    #include <QProcess>
#endif

SessionWindow::SessionWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SessionWindow), m_uiColumns(0)
{
    ui->setupUi(this);
    m_pSession = nullptr;
    m_pImageList = new QList<Image*>();

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
        ImageWorker w(path);
        connect(&w, &ImageWorker::imageReady, this, &SessionWindow::imageLoaded);
        w.run();
        m_pSession->incrementImages();        
    });

    m_pImageGrid = qobject_cast<QGridLayout *>(ui->imageContainer->layout());
}

SessionWindow::~SessionWindow()
{   
    delete ui;
    delete m_pImageList;
    delete m_pSession;
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
    qDebug() << "spacing" << ui->imageContainer->layout()->spacing();
    m_uiColumns = ceil( ui->imageContainer->width() / (THUMB_WIDTH + 2 * ui->imageContainer->layout()->spacing()));
    QtConcurrent::run(this, &SessionWindow::loadImages);
}

void SessionWindow::hideEvent(QHideEvent * /* event */)
{
    qDebug() << "cleaning image container before: " << ui->imageContainer->children().count();
    qDeleteAll(ui->imageContainer->findChildren<QWidget *>());
    qDeleteAll(m_pImageList->begin(), m_pImageList->end());
    m_pImageList->clear();
    CameraController::instance()->uninit();
    qDebug() << "cleaning image container after: " << ui->imageContainer->children().count();
}

void SessionWindow::loadImages()
{    
    qDebug() << "try to scan " << m_pSession->savePath();
    QDir dir(m_pSession->savePath(), NULL, QDir::Time|QDir::Reversed);
    if(dir.exists() == false)
        return;

    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    qDebug() << "Scanning: " << dir.path();

    QFileInfoList files = dir.entryInfoList();

    for(int i = 0; i < files.count(); i++)
    {
        QFileInfo info = files[i];
        qDebug() << "files: " << info.absoluteFilePath();
        ImageWorker w(info.absoluteFilePath());
        connect(&w, &ImageWorker::imageReady, this, &SessionWindow::imageLoaded);
        w.run();
    }
}

void SessionWindow::imageLoaded(Image *img)
{

    if (img->isValid() == false)
        return;

    int x = m_pImageList->count() % m_uiColumns;
    int y = m_pImageGrid->rowCount() - 1;
    if (x == 0)
        y++;


    ClickableLabel *lblTest = new ClickableLabel(ui->imageContainer);
    lblTest->setObjectName("lblThumbnail");
    lblTest->setFixedSize(THUMB_WIDTH, THUMB_HEIGHT);
    lblTest->setScaledContents(true);
    lblTest->setPixmap(*img->thumbnail());
    lblTest->setIndex(m_pImageList->count());
    connect(lblTest, &ClickableLabel::clicked, this, &SessionWindow::imageClicked);
    m_pImageGrid->addWidget(lblTest, y, x , Qt::AlignCenter);
    m_pImageList->append(img);
}

void SessionWindow::imageClicked()
{
    ClickableLabel *lbl = qobject_cast<ClickableLabel*>(sender());
    qDebug() << "Clicked on label: " << lbl->index() << " equals image: " << m_pImageList->at(lbl->index())->path();
}
