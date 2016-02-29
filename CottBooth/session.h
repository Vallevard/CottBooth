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

    static inline QString randomString(size_t length) {

        auto randChar = []() -> char
        {
            const char charset[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[ rand() % max_index ];
        };

        QString s(length, 0);
        std::generate_n(s.begin(),length,randChar);
        return s;
    }

    QString credentials();

private:
    QUuid   m_uidSid;
    QString m_sName;
    QString m_sPath;
    QString m_sCredentals;
};

#endif // SESSION_H
