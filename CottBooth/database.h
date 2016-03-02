#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include "session.h"

class Database : public QObject
{
    Q_OBJECT
public:
    static Database* instance();
    bool isInitialized();
    bool insertSession(Session *session);
    bool updateSession(Session *session);
    Session* selectSession(QString credentials);

    template<typename T>
    bool updateField(QString uid, QString field, T val)
    {
        QSqlQuery query;
        //necessary to bind field like this otherwise the string getting quoted
        query.prepare("UPDATE sessions SET " + field + " = :value WHERE uid = :uid");
        query.bindValue(":value", val);
        query.bindValue(":uid", uid);

        bool ret = query.exec();
        if(ret == false)
            qDebug() << "Couldn\'t update the session.\nError: " << query.lastError() << "\n" << query.lastQuery();

        return ret;
    }


private:
    const short VERSION = 1;
    const QString CREATE_STATEMENT = (
                "CREATE TABLE sessions ("
                "uid CHAR(36) PRIMARY KEY NOT NULL, "
                "name TEXT NOT NULL, "
                "path TEXT NOT NULL, "
                "credentials TEXT NOT NULL, "
                "date TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
                "images INTEGER DEFAULT 0, "
                "prints INTEGER DEFAULT 0, "
                "mails INTEGER DEFAULT 0, "
                "closed INTEGER DEFAULT 0"
                ");"
                );
    QSqlDatabase m_db;
    static Database *m_pInstance;
    bool m_bInitialized;

    explicit Database(QObject *parent = 0);
    void setup();

signals:

public slots:
};

#endif // DATABASE_H
