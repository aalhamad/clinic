#include "invoicedialog.h"

InvoiceDialog::InvoiceDialog(QSqlRelationalTableModel *model, QWidget *parent)
              :QDialog(parent), invoiceModel(model)
{
    initSetup();
    connect(okButton, SIGNAL(clicked()), this, SLOT(addInvoice()));
}

InvoiceDialog::InvoiceDialog(QSqlRelationalTableModel *model,
                             QSqlRecord &record, int row, QWidget *parent)
              :QDialog(parent), invoiceModel(model), invoiceRecord(record), invoiceRow(row)

{
    initSetup();
    fillFieldInputs();
    connect(okButton, SIGNAL(clicked()), this, SLOT(editInvoice()));

}

void InvoiceDialog::initSetup()
{
    setupLayout();
    setupInvoiceModel();
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

void InvoiceDialog::setupLayout()
{

    patientComboBox = new QComboBox;
    visitComboBox = new QComboBox;

    dateDateEdit = new QDateEdit;
    dateDateEdit->setDateTime(QDateTime::currentDateTime());
    dateDateEdit->setDisplayFormat("MMMM dd yyyy");
    dateDateEdit->setCalendarPopup(true);
    timeTimeEdit = new QTimeEdit;
    timeTimeEdit->setDateTime(QDateTime::currentDateTime());

    priceLineEdit = new QLineEdit;
    priceLineEdit->setValidator(new QDoubleValidator(this));

    okButton = new QPushButton(tr("&OK"));
    cancelButton = new QPushButton(tr("&Cancel"));
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(okButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

    QFormLayout *formLayout = new QFormLayout;
    QString asterisk = "<i style=color:red>*</i>";
    formLayout->addRow(tr("Patient: "), patientComboBox);
    formLayout->addRow(tr("Date: "), dateDateEdit);
    formLayout->addRow(tr("Time: "), timeTimeEdit);
    formLayout->addRow(tr("Price: ") + asterisk, priceLineEdit);
    formLayout->addRow(tr("Vist Type: "), visitComboBox);
    formLayout->addWidget(buttonBox);
    setLayout(formLayout);
}

void InvoiceDialog::setupInvoiceModel()
{
    QSqlDatabase db = invoiceModel->database();
    patientModel = new QSqlTableModel(this, db);
    patientModel->setTable("patients");
    patientModel->select();
    patientComboBox->clear();
    for(int i = 0; i < patientModel->rowCount(); i++)
    {
        patientComboBox->addItem(patientModel->record(i).field("full_name").value().toString(),
                                 patientModel->record(i).field("id").value());
    }

    visitModel = new QSqlTableModel(this, db);
    visitModel->setTable("visits");
    visitModel->select();
    visitComboBox->clear();
    for(int i = 0; i < visitModel->rowCount(); i++)
    {
        visitComboBox->addItem(visitModel->record(i).field("type").value().toString(),
                               visitModel->record(i).field("id").value());
    }

}

void InvoiceDialog::addInvoice()
{
    QSqlRecord record;

    QSqlField id("id", QVariant::Int);
    QSqlField date("date", QVariant::DateTime);
    QSqlField price("price", QVariant::Double);
    QSqlField patientId("patient_id", QVariant::Int);
    QSqlField visitId("visit_id", QVariant::Int);

    id.setAutoValue(true);
    date.setValue(QVariant(getDate()));
    price.setValue(QVariant(priceLineEdit->text()));
    patientId.setValue(QVariant(getPatientId()));
    visitId.setValue(QVariant(getVisitId()));

    record.append(id);
    record.append(date);
    record.append(price);
    record.append(patientId);
    record.append(visitId);


    if(isFieldInputValid() && invoiceModel->insertRecord(-1, record))
    {
        accept();
    }
    else
    {
        invoiceModel->revertAll();
    }
}

void InvoiceDialog::fillFieldInputs()
{

    dateDateEdit->setDate(invoiceRecord.field("date").value().toDateTime().date());
    timeTimeEdit->setTime(invoiceRecord.field("date").value().toDateTime().time());
    priceLineEdit->setText(invoiceRecord.field("price").value().toString());
    patientComboBox->setCurrentIndex(patientComboBox->findText(invoiceRecord
                                                               .field("full_name")
                                                               .value().toString()));
    visitComboBox->setCurrentIndex(visitComboBox->findText(invoiceRecord
                                                           .field("type")
                                                           .value().toString()));
}

void InvoiceDialog::editInvoice()
{
    invoiceRecord.setValue("date", QVariant(getDate()));
    invoiceRecord.setValue("price", QVariant(priceLineEdit->text()));

    QSqlField patientId("patient_id", QVariant::Int);
    patientId.setValue(QVariant(getPatientId()));

    QSqlField visitId("visit_id", QVariant::Int);
    visitId.setValue(QVariant(getVisitId()));

    invoiceRecord.append(patientId);
    invoiceRecord.append(visitId);

    if(isFieldInputValid() &&
       invoiceModel->setRecord(invoiceRow, invoiceRecord))
    {
        invoiceModel->submitAll();
        accept();
    }
    else
    {
        invoiceModel->revertAll();
    }
}

bool InvoiceDialog::isFieldInputValid()
{
    if(patientModel->rowCount() <= 0)
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("There are no patients. Please add some patients"));
        return false;
    }

    if (priceLineEdit->text().toDouble() < MINIMUM_PRICE )
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("Please enter a price greater than %1").arg(MINIMUM_PRICE));
        return false;
    }
    return true;
}

QDateTime InvoiceDialog::getDate() const
{
   QDateTime datetime;
   datetime.setDate(dateDateEdit->date());
   datetime.setTime(timeTimeEdit->time());
   return datetime;
}

int InvoiceDialog::getPatientId() const
{
    int patientId = patientComboBox->itemData(patientComboBox->currentIndex()).toInt();
    return patientId;
}

int InvoiceDialog::getVisitId() const
{
    int visitId = visitComboBox->itemData(visitComboBox->currentIndex()).toInt();
    return visitId;
}
