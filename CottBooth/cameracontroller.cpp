#include "cameracontroller.h"
#include <QDebug>
#include <QtConcurrent>


CameraController* CameraController::s_pInstance = nullptr;

CameraController* CameraController::instance()
{
    if(s_pInstance == nullptr)
    {
        s_pInstance = new CameraController();
    }

    return s_pInstance;
}

CameraController::CameraController(QObject *parent) : QObject(parent)
{
    gp_camera_new(&this->m_pCamera);
    this->m_pContext = gp_context_new();
}

CameraController::~CameraController() {
    this->m_bInitialized = false;
    gp_camera_unref(this->m_pCamera);
}

void CameraController::init()
{
    int ret = gp_camera_init(this->m_pCamera, this->m_pContext);
    if ( ret < GP_OK)
    {
        QString message = QString(gp_result_as_string(ret));
        emit initializationFailed(ret, message);
    } else {
        this->m_bInitialized = true;
        emit ready();
        QtConcurrent::run(this, &CameraController::startListening);
    }
}

void CameraController::startListening()
{
    int ret;
    void *data = NULL;
    CameraEventType evtype;

    while(m_bInitialized)
    {
        evtype = GP_EVENT_UNKNOWN;
        ret = gp_camera_wait_for_event(this->m_pCamera, 100, &evtype, &data, this->m_pContext);
        qDebug() << "tethering running: " << ret;

        if(ret < GP_OK)
        {
            //TODO: EMIT CameraInvalid
            return;
        }

        if(evtype == GP_EVENT_FILE_ADDED) {
            emit imageCreated(*static_cast<CameraFilePath*>(data));
        }
    }

    free(data);
}


