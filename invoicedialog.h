#ifndef INVOICEDIALOG_H
#define INVOICEDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QComboBox>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlRecord>
#include <QDateEdit>
#include <QTimeEdit>
#include <QLineEdit>
#include <QFormLayout>
#include <QSqlField>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

class InvoiceDialog : public QDialog
{
    Q_OBJECT
public:
    InvoiceDialog(QSqlRelationalTableModel *model, QWidget *parent = 0);
    InvoiceDialog(QSqlRelationalTableModel *model, QSqlRecord &record, int row, QWidget *parent = 0);

private slots:
    void addInvoice();
    void editInvoice();

private:
    void initSetup();
    void setupLayout();
    void setupInvoiceModel();
    bool isFieldInputValid();
    void fillFieldInputs();


    QDateTime getDate() const;
    int getPatientId() const;
    int getVisitId() const;

    QDateEdit *dateDateEdit;
    QTimeEdit *timeTimeEdit;

    QComboBox *patientComboBox;
    QComboBox *visitComboBox;

    QPushButton *okButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;
    QLineEdit *priceLineEdit;

    QSqlRelationalTableModel *invoiceModel;
    QSqlTableModel *patientModel;
    QSqlTableModel *visitModel;

    QSqlRecord invoiceRecord;
    int invoiceRow;

    static const double MINIMUM_PRICE = 0.1;

};

#endif // INVOICEDIALOG_H
