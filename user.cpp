#include "user.h"

User::User()
{
}

User::~User()
{

}

 bool User::authenticate(const QString& username, const QString& password, QSqlDatabase db)
{
     QSqlQuery query(db);
     query.prepare("SELECT hashed_password, salt FROM users WHERE username = :username");
     query.bindValue(":username", username);
     query.exec();
     query.next();

     QString hashed_password = query.value(0).toString();
     QString salt = query.value(1).toString();

     bool success = hashed_password == QCryptographicHash::hash(password.toAscii() + salt.toAscii(),
                                                                QCryptographicHash::Sha1);
     if (success)
     {
         return true;
     }

     return false;
}

 User User::find(const QString& username, QSqlDatabase db)
{
     QSqlQuery query(db);
     query.prepare("SELECT id, username, email, hashed_password, salt, role_id FROM users WHERE username = :username");
     query.bindValue(":username", username);
     query.exec();
     query.next();

     User user;

     user.m_id = query.value(0).toInt();
     user.m_username = query.value(1).toString();
     user.m_email = query.value(2).toString();
     user.m_hashedPassword = query.value(3).toString();
     user.m_salt = query.value(4).toString();
     user.m_role = query.value(5).toInt();
     return user;
}

QString User::username() const
{
    return m_username;
}

int User::id() const
{
    return m_id;
}

int User::role() const
{
    return m_role;
}

QString User::email() const
{
    return m_email;
}

QString User::hashedPassword() const
{
    return m_hashedPassword;
}

QString User::salt() const
{
    return m_salt;
}
