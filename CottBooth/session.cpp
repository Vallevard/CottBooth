#include "session.h"
#include "settings.h"

Session::Session(QString sessionName, QString savePath)
{
    m_sName = sessionName;
    m_sPath = savePath;
    m_uidSid = QUuid::createUuid();
    m_sCredentals = randomString(Settings::instance()->intValue(Settings::CREDENTIAL_LENGTH));
}


QString Session::credentials()
{
    return m_sCredentals;
}
