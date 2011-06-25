#include "invoice.h"

Invoice::Invoice(QObject *parent) :
    QObject(parent)
{
}

Invoice::Invoice(QString &fullname, QDateTime &datetime, QString &price, QString &visitType)
        :m_fullname(fullname), m_datetime(m_datetime), m_price(price), m_visteType(visitType)
{}

