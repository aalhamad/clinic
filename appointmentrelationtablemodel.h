#ifndef APPOINTMENTRELATIONTABLEMODEL_H
#define APPOINTMENTRELATIONTABLEMODEL_H

#include <QSqlRelationalTableModel>
#include <QDateTime>

class Appointmentrelationtablemodel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
     explicit Appointmentrelationtablemodel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex &item, int role) const;
};

#endif // APPOINTMENTRELATIONTABLEMODEL_H
