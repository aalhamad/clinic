#include "patientdialog.h"

PatientDialog::PatientDialog(QSqlRelationalTableModel *model, QWidget *parent)
              : QDialog(parent), patientModel(model)
{
    initSetup();
    connect(okButton, SIGNAL(clicked()), this, SLOT(addPatient()));
}

PatientDialog::PatientDialog(QSqlRelationalTableModel *model,
                             QSqlRecord &record, int row, QWidget *parent)
              :QDialog(parent), patientModel(model), patientRecord(record), patientRow(row)
{
    initSetup();
    fillFieldInputs();
    connect(okButton, SIGNAL(clicked()), this, SLOT(editPatient()));
}

void PatientDialog::initSetup()
{
    setupLayout();
    setupPatientModel();
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

void PatientDialog::setupLayout()
{
    okButton = new QPushButton(tr("&OK"));
    cancelButton = new QPushButton(tr("&Cancel"));
    buttonBox = new QDialogButtonBox;

    buttonBox->addButton(okButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

    fileNumberLineEdit = new QLineEdit;
    firstNameLineEdit = new QLineEdit;
    lastNameLineEdit = new QLineEdit;
    genderComboBox = new QComboBox;
    countryComboBox = new QComboBox;
    civilIdLineEdit = new QLineEdit;
    firstPhoneNumberLineEdit = new QLineEdit;
    secondPhoneNumberLineEdit = new QLineEdit;
    emailLineEdit = new QLineEdit;

    /* Validations */
    QValidator *intValidator = new QIntValidator(this);
    fileNumberLineEdit->setValidator(intValidator);
    civilIdLineEdit->setValidator(intValidator);
    firstPhoneNumberLineEdit->setValidator(intValidator);
    secondPhoneNumberLineEdit->setValidator(intValidator);

    QFormLayout *formLayout = new QFormLayout;
    QString asterisk = "<i style=color:red>*</i>";
    formLayout->addRow(tr("File Number: ") + asterisk, fileNumberLineEdit);
    formLayout->addRow(tr("First &Name: ") + asterisk, firstNameLineEdit);
    formLayout->addRow(tr("&Last Name: ") + asterisk, lastNameLineEdit);
    formLayout->addRow(tr("&Gender:"), genderComboBox);
    formLayout->addRow(tr("&Country"), countryComboBox);
    formLayout->addRow(tr("C&ivil Id:"), civilIdLineEdit);
    formLayout->addRow(tr("Phone Number &1: ") + asterisk, firstPhoneNumberLineEdit);
    formLayout->addRow(tr("Phone Number &2:"), secondPhoneNumberLineEdit);
    formLayout->addRow(tr("&Email"), emailLineEdit);
    formLayout->addWidget(buttonBox);

    setLayout(formLayout);
}

void PatientDialog::setupPatientModel()
{
   QSqlDatabase db = patientModel->database();
   countryModel = new QSqlTableModel(this, db);
   countryModel->setTable("countries");
   countryModel->select();
   countryComboBox->setModel(countryModel);
   countryComboBox->setModelColumn(1);

   QStringList genders;
   genders << "Male" << "Female";
   genderComboBox->addItems(genders);
}

void PatientDialog::fillFieldInputs()
{
    fileNumberLineEdit->setText(patientRecord.field("file_number").value().toString());
    firstNameLineEdit->setText(patientRecord.field("first_name").value().toString());
    lastNameLineEdit->setText(patientRecord.field("last_name").value().toString());
    civilIdLineEdit->setText(patientRecord.field("civil_id_number").value().toString());
    firstPhoneNumberLineEdit->setText(patientRecord.field("first_phone_number").value().toString());
    secondPhoneNumberLineEdit->setText(patientRecord.field("second_phone_number").value().toString());
    emailLineEdit->setText(patientRecord.field("email").value().toString());

    if(patientRecord.field("gender").value().toString() == "Male")
    {
        genderComboBox->setCurrentIndex(Male);
    }
    else
    {
        genderComboBox->setCurrentIndex(Female);
    }

    countryComboBox->setCurrentIndex(countryComboBox->findText(patientRecord.field("name").value().toString()));

}

bool PatientDialog::isFieldInputValid()
{

    if(fileNumberLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("File number can't be empty."));
        return false;
    }
    if(firstNameLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("Fist name can't be empty."));
        return false;
    }

    if(lastNameLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("Last name can't be empty."));
        return false;
    }

    if(firstPhoneNumberLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("First phone number can't be empty."));
        return false;
    }
    return true;

}

bool PatientDialog::patientExists()
{

    QSqlQuery query(patientModel->database());
    query.prepare("SELECT first_name, last_name FROM patients "
                  "WHERE first_name = :first_name AND last_name = :last_name" );
    query.bindValue(":first_name", firstNameLineEdit->text());
    query.bindValue(":last_name", lastNameLineEdit->text());
    query.exec();
    query.next();

    if(query.isValid())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("Patient already exists."));
        return true;
    }
    return false;
}

bool PatientDialog::fileNumberExists()
{

    QSqlQuery query(patientModel->database());
    query.prepare("SELECT file_number FROM patients WHERE file_number = :file_number");
    query.bindValue(":file_number", fileNumberLineEdit->text());
    query.exec();
    query.next();

    qDebug() << query.value(0);

    if(query.isValid())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("File number already exists."));
        return true;
    }
    return false;
}

void PatientDialog::addPatient()
{
    QSqlRecord record;

    QSqlField id("id", QVariant::Int);
    QSqlField fileNumber("file_number", QVariant::String);
    QSqlField firstName("first_name", QVariant::String);
    QSqlField lastName("last_name", QVariant::String);
    QSqlField fullName("full_name", QVariant::String);
    QSqlField gender("gender", QVariant::String);
    QSqlField countryId("country_id", QVariant::Int);
    QSqlField civilIdNumber("civil_id_number", QVariant::String);
    QSqlField firstPhoneNumber("first_phone_number", QVariant::String);
    QSqlField secondPhoneNumber("second_phone_number", QVariant::String);
    QSqlField email("email", QVariant::String);
    QSqlField createdAt("created_at", QVariant::DateTime);
    QSqlField updatedAt("updated_at", QVariant::DateTime);

    QVariant countryIdFromBox = countryModel->record(countryComboBox->currentIndex())
                                .field("id").value();

    id.setAutoValue(true);
    fileNumber.setValue(QVariant(fileNumberLineEdit->text()));
    firstName.setValue(QVariant(firstNameLineEdit->text()));
    lastName.setValue(QVariant(lastNameLineEdit->text()));
    fullName.setValue(QVariant(firstNameLineEdit->text() + " " + lastNameLineEdit->text()));
    gender.setValue(QVariant(genderComboBox->currentText()));
    countryId.setValue(countryIdFromBox);
    civilIdNumber.setValue(QVariant(civilIdLineEdit->text()));
    firstPhoneNumber.setValue(QVariant(firstPhoneNumberLineEdit->text()));
    secondPhoneNumber.setValue(QVariant(secondPhoneNumberLineEdit->text()));
    email.setValue(QVariant(emailLineEdit->text()));
    createdAt.setValue(QVariant(QDateTime::currentDateTime()));
    updatedAt.setValue(QVariant(QDateTime::currentDateTime()));

    record.append(id);
    record.append(fileNumber);
    record.append(firstName);
    record.append(lastName);
    record.append(fullName);
    record.append(gender);
    record.append(countryId);
    record.append(civilIdNumber);
    record.append(firstPhoneNumber);
    record.append(secondPhoneNumber);
    record.append(email);
    record.append(createdAt);
    record.append(updatedAt);

    if(isFieldInputValid()
        && !patientExists()
        && !fileNumberExists()
        && patientModel->insertRecord(-1, record))
    {
        accept();
    }
    else
    {
        patientModel->revertAll();
    }
}

void PatientDialog::editPatient()
{
    patientRecord.setValue("file_number", QVariant(fileNumberLineEdit->text()));
    patientRecord.setValue("first_name", QVariant(firstNameLineEdit->text()));
    patientRecord.setValue("last_name", QVariant(lastNameLineEdit->text()));
    patientRecord.setValue("full_name", QVariant(firstNameLineEdit->text() + " " + lastNameLineEdit->text()));
    patientRecord.setValue("gender", QVariant(genderComboBox->currentText()));
    patientRecord.setValue("civil_id_number", QVariant(civilIdLineEdit->text()));
    patientRecord.setValue("first_phone_number", QVariant(firstPhoneNumberLineEdit->text()));
    patientRecord.setValue("second_phone_number", QVariant(secondPhoneNumberLineEdit->text()));
    patientRecord.setValue("email", QVariant(emailLineEdit->text()));
    patientRecord.setValue("updated_at", QVariant(QDateTime::currentDateTime()));

    QSqlField countryId("country_id", QVariant::Int);
    QVariant countryIdFromBox = countryModel->record(countryComboBox->currentIndex())
                                .field("id").value();
    countryId.setValue(countryIdFromBox);
    patientRecord.append(countryId);
    patientRecord.setValue("country_id", countryIdFromBox);

    if(isFieldInputValid() && patientModel->setRecord(patientRow, patientRecord))
    {
        patientModel->submitAll();
        accept();
    }
    else
    {
        patientModel->revertAll();
    }

}
