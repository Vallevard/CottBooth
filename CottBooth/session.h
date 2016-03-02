#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QUuid>
#include <algorithm>

class Session :QObject
{
    Q_OBJECT

public:
    Session(QString sessionName, QString savePath);
    Session(QString sid, QString sessionName,
            QString savePath, QString credentials,
            uint images, uint prints, uint mails);

    static QString randomString(const int len);

    QString sessionId();
    QString credentials();
    QString sessionName();
    QString savePath();
    uint numberOfMails();
    uint numberOfPrints();
    uint numberOfImages();

    void incrementImages();
    void incrementPrints();
    void incrementMails();
    void close();

private:
    QString m_sSid;
    QString m_sName;
    QString m_sPath;
    QString m_sCredentals;
    uint m_uiNumberOfImages;
    uint m_uiNumberOfPrints;
    uint m_uiNumberOfMails;
};

#endif // SESSION_H
