#include "invoicerelationtablemodel.h"

InvoiceRelationTableModel::InvoiceRelationTableModel(QObject *parent, QSqlDatabase db)
             :QSqlRelationalTableModel(parent, db)
{

}

QVariant InvoiceRelationTableModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole && item.column() == 2 )
    {
        QDateTime date  = QSqlTableModel::data(item, role).toDateTime();
        return date.toString("MMMM dd yyyy h:mm AP");
    }

    else
    {
        return QSqlTableModel::data(item, role);
    }
}

