#include "database.h"
#include <QApplication>
#include <QDebug>

Database* Database::m_pInstance = nullptr;

Database* Database::instance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new Database();
    }

    return m_pInstance;
}

Database::Database(QObject *parent) : QObject(parent)
{
    QString dbPath = QApplication::applicationDirPath() + QDir::separator() + "cottbooth.db";
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if(!m_db.open())
    {
       qDebug() << "Couldnt open db";
    }

    setup();

    qDebug() << "initialized: " << m_bInitialized;
}

void Database::setup()
{
    if(m_db.tables().contains("sessions"))
    {
        this->m_bInitialized = true;
        return;
    }

    QSqlQuery query;
    if(query.exec(CREATE_STATEMENT) == false)
    {
        qDebug() << "Error create table: " << query.lastError() << "\n Query:";
        return;
    }

    if(query.exec("PRAGMA user_version=" + QString::number(VERSION) + ";") == false)
    {
        qDebug() << "Error set PRAGMA user_version: " << query.lastError() << "\n Query:";
        return;
    }

    this->m_bInitialized = true;
}

bool Database::isInitialized()
{
    return m_bInitialized;
}

bool Database::insertSession(Session *session)
{
    QSqlQuery query;
    QString q("INSERT INTO sessions "
              "(uid,name,path,credentials,images,prints,mails) VALUES "
              "(:uid,:name,:path,:credentials,:images,:prints,:mails);");
    query.prepare(q);
    query.bindValue(":uid", session->sessionId());
    query.bindValue(":name", session->sessionName());
    query.bindValue(":path", session->savePath());
    query.bindValue(":credentials", session->credentials());
    query.bindValue(":images", session->numberOfImages());
    query.bindValue(":prints", session->numberOfPrints());
    query.bindValue(":mails", session->numberOfMails());

    bool ret = query.exec();
    if(ret == false)
        qDebug() << "Couldn\'t insert the session.\nError: " << query.lastError();

    return ret;
}

bool Database::updateSession(Session *session)
{
    QSqlQuery query;
    QString q("UPDATE sessions SET"
              "name = :name, path = :path, credentials = :credentials,"
              "images = :images, prints = :prints, mails = :mails "
              "WHERE uid = :sid;"
              );
    query.prepare(q);
    query.bindValue(":name", session->sessionName());
    query.bindValue(":path", session->savePath());
    query.bindValue(":credentials", session->credentials());
    query.bindValue(":images", session->numberOfImages());
    query.bindValue(":prints", session->numberOfPrints());
    query.bindValue(":mails", session->numberOfMails());
    query.bindValue(":sid", session->sessionId());

    bool ret = query.exec();
    if(ret == false)
        qDebug() << "Couldn\'t update the session.\nError: " << query.lastError();

    return ret;
}

Session* Database::selectSession(QString credentials)
{
    QSqlQuery query;    
    query.prepare("SELECT uid, name, path, credentials, images, prints, mails"
                  " FROM sessions WHERE credentials = :credentials and closed = 0;");
    query.bindValue(":credentials", credentials);

    if(query.exec() == false)
    {
        qDebug() << "Couldn\'t select the session.\nError: " << query.lastError();
        return nullptr;
    }

    if(!query.first())
    {
        qDebug() << "Couldn\'t  find a record .\nError: " << query.lastError();
        return nullptr;
    }

    return new Session(query.value(0).toString(), query.value(1).toString(),
                       query.value(2).toString(), query.value(3).toString(),
                       query.value(5).toUInt(), query.value(6).toUInt(),
                       query.value(7).toUInt());

}
