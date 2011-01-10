#ifndef USER_H
#define USER_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QVariant>
#include <QCryptographicHash>

class User
{
public:
    User();
    ~User();

    static bool authenticate(const QString& username, const QString& password, QSqlDatabase db = QSqlDatabase());
    static User find(const QString& username, QSqlDatabase db = QSqlDatabase());
    QString username() const;
    int id() const;
    int role() const;
    QString email() const;
    QString hashedPassword() const;
    QString salt() const;
    enum { Admin = 1, Operator = 2, Viewer = 3 };

private:
    QString m_username;
    int m_role;
    int m_id;
    QString m_salt;
    QString m_hashedPassword;
    QString m_email;
};

#endif // USER_H
