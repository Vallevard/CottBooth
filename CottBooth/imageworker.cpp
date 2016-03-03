#include "imageworker.h"


ImageWorker::ImageWorker(QString image, QObject *parent) :
    QThread(parent), m_sImage(image)
{

}

void ImageWorker::run()
{
    Image *img = new Image(m_sImage);
    if(img->isValid())
        emit imageReady(img);
}
