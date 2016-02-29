#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QObject>
#include <gphoto2/gphoto2-camera.h>

namespace CottBooth
{
    class CameraController;
}

class CameraController : public QObject
{
    Q_OBJECT
public:    
    static CameraController *instance();
    void init();

private:
    CameraController(QObject *parent = 0);
    ~CameraController();

    static CameraController *s_pInstance;
    Camera *m_pCamera;
    GPContext *m_pContext;
    bool m_bInitialized = false;
    void startListening();

signals:
    void ready();
    void initializationFailed(int errCode, QString message);
    void imageCreated(CameraFilePath path);

public slots:

};

#endif // CAMERACONTROLLER_H
