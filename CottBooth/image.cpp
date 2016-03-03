#include "image.h"
#include <exiv2.hpp>
#include <QDir>
#include <QDebug>
#include <QPixmap>
#include <assert.h>


Image::Image(QString path, QObject *parent) : QObject(parent),
    m_bValid(false)
{
    if (QFile::exists(path) == false)
        throw std::runtime_error(tr("File not exist.").toStdString());

    m_sImage = path;
    openThumbnail();
}

void Image::openThumbnail()
{
    try {

        Exiv2::Image::AutoPtr img = Exiv2::ImageFactory::open(m_sImage.toStdString());
        assert(img.get() != 0);
        img->readMetadata();

        if(QString::fromStdString(img->mimeType()).compare("image/jpeg") == 0)
            m_bJPEG = true;

        Exiv2::ExifData &exifData = img->exifData();
        if (exifData.empty()) {
            qDebug() << "exif data are empty";
            return;
        }

        Exiv2::PreviewManager loader(*img);
        Exiv2::PreviewPropertiesList list = loader.getPreviewProperties();
        qDebug() << "number of thumbnails in exif: " << list.size();

        for(Exiv2::PreviewPropertiesList::iterator it = list.begin(); it != list.end(); ++it)
        {
            Exiv2::PreviewProperties item = *it;
            qDebug() << "Preview Item: " << item.width_ << "x" << item.height_ << " ext:" << QString::fromStdString(item.extension_);
        }

        Exiv2::PreviewImage preview = loader.getPreviewImage(*list.begin());
        if(preview.size() <= 0){
            qDebug() << "exif thumbnail is empty";
            return;
        }



        Exiv2::DataBuf buf = preview.copy();
        const char* rawData = new char[buf.size_];
        memcpy((void*)rawData, buf.pData_, buf.size_);

        QByteArray data(rawData, buf.size_);
        m_pThumbnail = new QPixmap();
        if ( m_pThumbnail->loadFromData(data) )
            m_bValid = true;

        buf.release();
        delete rawData;

    }
    catch (Exiv2::Error& e)
    {
        qDebug() << "error in image: " << e.what();
    }
}

Image::~Image()
{
    qDebug() << "delete image " << m_sImage;
    delete m_pThumbnail;
    delete m_pImage;
}

bool Image::isValid()
{
    return m_bValid;
}

QPixmap* Image::thumbnail()
{
    if (m_bValid)
        return m_pThumbnail;

    return nullptr;
}

QString Image::path()
{
    return m_sImage;
}

void Image::releaseImage()
{
    delete m_pImage;
}

QPixmap* Image::image()
{
    if(m_pImage != nullptr)
        return m_pImage;

    if(m_bJPEG)
    {
        m_pImage = new QPixmap();
        m_pImage->fromImage(QImage(m_sImage));
    } else {

    }

    return m_pImage;
}
