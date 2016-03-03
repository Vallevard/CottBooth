#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>

namespace CottBooth {
class Image;
}

class Image : public QObject
{
    Q_OBJECT
public:
    explicit Image(QString path, QObject *parent = 0);
    ~Image();
    bool isValid();
    bool isJPEG();
    QPixmap* thumbnail();
    QPixmap* image();
    QString path();
    void releaseImage();

private:
    QString m_sImage;
    QPixmap *m_pThumbnail;
    QPixmap *m_pImage;
    bool m_bJPEG;
    bool m_bValid;
    void openThumbnail();

signals:

public slots:
};

#endif // IMAGE_H
