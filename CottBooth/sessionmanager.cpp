#include "sessionmanager.h"
#include "database.h"

Session* SessionManager::createSession(QString name, QString path)
{
    return new Session(name, path);
}

Session* SessionManager::respawnSession(QString credentials)
{
    Database *db = Database::instance();
    return db->selectSession(credentials);
}
