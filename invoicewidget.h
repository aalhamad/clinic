#ifndef INVOICEWIDGET_H
#define INVOICEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTableView>
#include <QPushButton>
#include <QSqlRelationalTableModel>
#include <QStatusBar>
#include <QGridLayout>
#include <QSqlRelationalDelegate>
#include <QSqlRecord>
#include <QSqlField>
#include <QHeaderView>
#include <QMessageBox>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QSortFilterProxyModel>
#include <QFile>
#include "invoicedialog.h"
#include "user.h"

class InvoiceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InvoiceWidget(QSqlRelationalTableModel *model,
                           QStatusBar *statusBar,
                           User user,
                           QWidget *parent = 0);

    void updateTableView();

private slots:
    void addEntry();
    void editEntry();
    void deleteEntry();
    void print();
    void printPreview(QPrinter *printer);
    void onFilterLineEditChanged();
    void onFilterColumnChanged();

private:
    void setupLayout();
    void setupModel(QSqlRelationalTableModel *model);
    void setupTableView(QSqlRelationalTableModel *model);

    QLabel *headerLabel;
    QTableView *invoiceTableView;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *printButton;
    QComboBox *filterComboBox;
    QLineEdit *filterLineEdit;

    QSqlRelationalTableModel *invoiceModel;
    QStatusBar *mainWindowStatusBar;

    QSortFilterProxyModel *invoiceSortModel;

    enum {Id, Patient, Date, Price, Visit};
    enum { DEFAULT_PRINT_WIDTH = 25, DEFAULT_PRINT_HEIGH = 25 };

    User m_user;

};

#endif // INVOICEWIDGET_H
