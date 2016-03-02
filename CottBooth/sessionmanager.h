#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include "session.h"

class SessionManager : public QObject
{
    Q_OBJECT
public:
    static Session* createSession(QString name, QString path);
    static Session* respawnSession(QString credentials);

signals:

public slots:
};

#endif // SESSIONMANAGER_H
