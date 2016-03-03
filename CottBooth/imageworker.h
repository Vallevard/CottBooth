#ifndef IMAGEWORKER_H
#define IMAGEWORKER_H

#include <QThread>
#include "image.h"

class ImageWorker : public QThread
{
    Q_OBJECT
public:
    explicit ImageWorker(QString image, QObject *parent = 0);
    void run();

private:
    QString m_sImage;

signals:
    void imageReady(Image *image);

public slots:    
};

#endif // IMAGEWORKER_H
