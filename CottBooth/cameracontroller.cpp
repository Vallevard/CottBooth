#include "cameracontroller.h"
#include <QDebug>
#include <QtConcurrent>
#include <functional>
#include <fcntl.h>



CameraController* CameraController::s_pInstance = nullptr;

CameraController* CameraController::instance()
{
    if(s_pInstance == nullptr)
    {
        s_pInstance = new CameraController();
    }

    return s_pInstance;
}

CameraController::CameraController(QObject *parent) : QObject(parent) , m_Mutex()
{    
}

CameraController::~CameraController() {
    uninit();
}

void CameraController::init()
{
    gp_camera_new(&this->m_pCamera);
    this->m_pContext = gp_context_new();

    // necessary otherwise PTPCamera blocks the camera communication
    #ifdef Q_OS_MAC
        int result = system("pkill PTPCamera");
        qDebug() << "PTPCamera killed: ret " << result;
    #endif

    qDebug() << "Try to init camera| Listener running: " << this->m_CameraListener.isRunning();


    int ret = gp_camera_init(this->m_pCamera, this->m_pContext);



    if ( ret < GP_OK)
    {        
        return error(ret);
    }

    this->m_bInitialized = true;
    emit ready();

    if(this->m_bListening == false)
    {
        this->m_bListening = true;
        this->m_CameraListener = QtConcurrent::run(this, &CameraController::startListening);
    }
}

void CameraController::uninit()
{        
    m_Mutex.lock();
    this->m_bInitialized = false;
    this->m_bListening = false;
    m_Mutex.unlock();

    gp_camera_exit(this->m_pCamera, this->m_pContext);
}

void CameraController::startListening()
{
    emit tetheringBegin();

    int timeToWait = 100;
    int ret;
    void *data = NULL;
    CameraEventType evtype;

    while(isListening())
    {
        evtype = GP_EVENT_UNKNOWN;

        ret = gp_camera_wait_for_event(this->m_pCamera, timeToWait, &evtype, &data, this->m_pContext);
        if(ret < GP_OK)
        {
            error(ret);
            break;
        }

        if(evtype == GP_EVENT_FILE_ADDED) {
            CameraFilePath *path = static_cast<CameraFilePath*>(data);
            copyTo(path);
            emit captureComplete(m_sCopyToPath + QDir::separator() + path->name);
            free(data);
        }
    }

    emit tetheringEnd();
}

void CameraController::error(int errCode)
{
    emit fail(errCode, gp_result_as_string(errCode));
}

bool CameraController::isListening()
{
    bool value = false;

    m_Mutex.lock();
    value = m_bListening;
    m_Mutex.unlock();

    return value;
}

void CameraController::setCopyToPath(QString path)
{
    this->m_sCopyToPath = path;
}

void CameraController::copyTo(CameraFilePath *path)
{
    if(m_sCopyToPath == nullptr)
        return;

    QDir dest(m_sCopyToPath);
    if(dest.exists() == false && dest.mkpath(".") == false)
        return;


    int fd, ret = 0;
    CameraFile *file;
    QString filePath = dest.absolutePath() + QDir::separator() + path->name;
    fd = open(filePath.toStdString().c_str(), O_CREAT | O_WRONLY, 0644);
    ret = gp_file_new_from_fd(&file, fd);
    ret = gp_camera_file_get(this->m_pCamera,path->folder, path->name, GP_FILE_TYPE_NORMAL, file, this->m_pContext);

    //TODO: read from settings whether delete image from sd card
    ret = gp_camera_file_delete(this->m_pCamera, path->folder, path->name, this->m_pContext);
    gp_file_free(file);
}


