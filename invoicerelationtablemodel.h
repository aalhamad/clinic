#ifndef INVOICEMODEL_H
#define INVOICEMODEL_H

#include <QSqlRelationalTableModel>
#include <QDateTime>

class InvoiceRelationTableModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    explicit InvoiceRelationTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex &item, int role) const;
};

#endif // INVOICEMODEL_H
