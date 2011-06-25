#ifndef INVOICE_H
#define INVOICE_H

#include <QObject>
#include <QString>
#include <QDateTime>

class Invoice : public QObject
{
    Q_OBJECT
public:
    explicit Invoice(QObject *parent = 0);
    explicit Invoice(QString& fullname, QDateTime& datetime, QString&price, QString& visitType);

    QString getFullname() const { return m_fullname; }
    QDateTime getDateTime() const { return m_datetime; }
    QString getPrice() const { return m_price; }
    QString getVisitType() const { return m_visteType; }

private:
    QString   m_fullname;
    QDateTime m_datetime;
    QString   m_price;
    QString   m_visteType;
};

#endif // INVOICE_H
