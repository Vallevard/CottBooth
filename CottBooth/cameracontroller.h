#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QObject>
#include <gphoto2/gphoto2-camera.h>
#include <QFuture>
#include <QMutex>
#include <QDir>

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
    void uninit();    
    void setCopyToPath(QString path);

private:
    CameraController(QObject *parent = 0);
    ~CameraController();

    static CameraController *s_pInstance;
    Camera *m_pCamera;
    GPContext *m_pContext;
    QMutex m_Mutex;
    QFuture<void> m_CameraListener;
    QString m_sCopyToPath;

    bool m_bInitialized = false;
    bool m_bListening = false;
    void startListening();
    void error(int errCode);
    void copyTo(CameraFilePath *path);
    bool isListening();

signals:
    void ready();
    void fail(int errCode, QString message);
    void captureComplete(QString path);
    void copyComplete(QString path);
    void tetheringBegin();
    void tetheringEnd();

public slots:

};

#endif // CAMERACONTROLLER_H
