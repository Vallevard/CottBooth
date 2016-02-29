#include "session.h"

Session::Session(QString sessionName, QString savePath)
{
    m_sName = sessionName;
    m_sPath = savePath;
    m_uidSid = QUuid::createUuid();
    m_sCredentals = randomString(6);
}
