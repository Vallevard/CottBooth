#include "session.h"
#include "settings.h"
#include "database.h"

Session::Session(QString sessionName, QString savePath) :       
    m_sName(sessionName),m_uiNumberOfImages(0), m_uiNumberOfPrints(0), m_uiNumberOfMails(0)
{    
    m_sSid = QUuid::createUuid().toString().remove('{').remove('}');
    m_sPath = savePath + QDir::separator() + m_sSid;
    m_sCredentals = randomString(Settings::instance()->intValue(Settings::CREDENTIAL_LENGTH));
    Database::instance()->insertSession(this);
}

Session::Session(QString sid, QString sessionName,
        QString savePath, QString credentials,
        uint images, uint prints, uint mails)
{
    m_sSid = sid;
    m_sName = sessionName;
    m_sPath = savePath;
    m_sCredentals = credentials;
    m_uiNumberOfImages = images;
    m_uiNumberOfPrints = prints;
    m_uiNumberOfMails = mails;
}

QString Session::savePath()
{
    return m_sPath;
}

QString Session::sessionName()
{
    return m_sName;
}

QString Session::credentials()
{
    return m_sCredentals;
}

QString Session::sessionId()
{
    return m_sSid;
}

uint Session::numberOfImages()
{
    return m_uiNumberOfImages;
}

uint Session::numberOfMails()
{
    return m_uiNumberOfMails;
}

uint Session::numberOfPrints()
{
    return m_uiNumberOfPrints;
}

void Session::incrementImages()
{
    this->m_uiNumberOfImages++;
    Database::instance()->updateField(sessionId(), "images", this->m_uiNumberOfImages);
}

void Session::incrementPrints()
{
    this->m_uiNumberOfPrints++;
    Database::instance()->updateField(sessionId(), "prints", this->m_uiNumberOfPrints);
}

void Session::incrementMails()
{
    this->m_uiNumberOfMails++;
    Database::instance()->updateField(sessionId(), "mails", this->m_uiNumberOfMails);
}

QString Session::randomString(const int length) {
    srand(time(0));
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    QString str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

void Session::close()
{
    Database::instance()->updateField(sessionId(), "closed", 1);
}
