#include "employeedialog.h"

EmployeeDialog::EmployeeDialog(QSqlRelationalTableModel *model, QWidget *parent)
               : QDialog(parent), employeeModel(model)
{
    initSetup();
    connect(okButton, SIGNAL(clicked()), this, SLOT(addEmployee()));
}

EmployeeDialog::EmployeeDialog(QSqlRelationalTableModel *model,
                               QSqlRecord &record, int row, QWidget *parent)
               :QDialog(parent), employeeModel(model), employeeRecord(record), employeeRow(row)

{
    initSetup();
    fillFieldInputs();
    connect(okButton, SIGNAL(clicked()), this, SLOT(editEmployee()));
}

void EmployeeDialog::initSetup()
{
    setupLayout();
    setupEmployeeModel();
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}


void EmployeeDialog::setupLayout()
{
    okButton = new QPushButton(tr("&OK"));
    cancelButton = new QPushButton(tr("&Cancel"));
    buttonBox = new QDialogButtonBox;

    buttonBox->addButton(okButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

    firstNameLineEdit = new QLineEdit;
    lastNameLineEdit = new QLineEdit;
    genderComboBox = new QComboBox;
    occupationComboBox = new QComboBox;
    countryComboBox = new QComboBox;
    civilIdLineEdit = new QLineEdit;
    firstPhoneNumberLineEdit = new QLineEdit;
    secondPhoneNumberLineEdit = new QLineEdit;
    emailLineEdit = new QLineEdit;

    /* Validations */
    QValidator *intValidator = new QIntValidator(this);
    civilIdLineEdit->setValidator(intValidator);
    firstPhoneNumberLineEdit->setValidator(intValidator);
    secondPhoneNumberLineEdit->setValidator(intValidator);

    QFormLayout *formLayout = new QFormLayout;
    QString asterisk = "<i style=color:red>*</i>";
    formLayout->addRow(tr("First &Name: ") + asterisk, firstNameLineEdit);
    formLayout->addRow(tr("&Last Name: ") + asterisk, lastNameLineEdit);
    formLayout->addRow(tr("&Gender"), genderComboBox);
    formLayout->addRow(tr("&Occupation"), occupationComboBox);
    formLayout->addRow(tr("&Country"), countryComboBox);
    formLayout->addRow(tr("C&ivil Id:"), civilIdLineEdit);
    formLayout->addRow(tr("Phone Number &1: ") + asterisk, firstPhoneNumberLineEdit);
    formLayout->addRow(tr("Phone Number &2:"), secondPhoneNumberLineEdit);
    formLayout->addRow(tr("&Email"), emailLineEdit);
    formLayout->addWidget(buttonBox);

    setLayout(formLayout);

}

void EmployeeDialog::setupEmployeeModel()
{
    /* Contries */
    QSqlDatabase db = employeeModel->database();
    countryModel = new QSqlTableModel(this, db);
    countryModel->setTable("countries");
    countryModel->select();
    countryComboBox->setModel(countryModel);
    countryComboBox->setModelColumn(1);

    /* Genders */
    QStringList genders;
    genders << "Male" << "Female";
    genderComboBox->addItems(genders);

    /* Occupation */
    occupationModel = new QSqlTableModel(this, db);
    occupationModel->setTable("occupations");
    occupationModel->select();
    occupationComboBox->setModel(occupationModel);
    occupationComboBox->setModelColumn(1);

}

void EmployeeDialog::fillFieldInputs()
{
    firstNameLineEdit->setText(employeeRecord.field("first_name").value().toString());
    lastNameLineEdit->setText(employeeRecord.field("last_name").value().toString());
    civilIdLineEdit->setText(employeeRecord.field("civil_id_number").value().toString());
    firstPhoneNumberLineEdit->setText(employeeRecord.field("first_phone_number").value().toString());
    secondPhoneNumberLineEdit->setText(employeeRecord.field("second_phone_number").value().toString());
    emailLineEdit->setText(employeeRecord.field("email").value().toString());

    if(employeeRecord.field("gender").value().toString() == "Male")
    {
        genderComboBox->setCurrentIndex(Male);
    }
    else
    {
        genderComboBox->setCurrentIndex(Female);
    }

    countryComboBox->setCurrentIndex(countryComboBox->findText(employeeRecord.field("name").value().toString()));
    occupationComboBox->setCurrentIndex(occupationComboBox->findText(employeeRecord.field("type").value().toString()));

}

bool EmployeeDialog::isFieldInputValid()
{
    if(firstNameLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("First name can't be empty."));
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
                                 tr("First phone can't be empty."));
        return false;
    }

    return true;
}

bool EmployeeDialog::employeeExists()
{
    QSqlQuery query(employeeModel->database());
    query.prepare("SELECT first_name, last_name FROM employees "
                  "WHERE first_name = :first_name AND last_name = :last_name" );
    query.bindValue(":first_name", firstNameLineEdit->text());
    query.bindValue(":last_name", lastNameLineEdit->text());
    query.exec();
    query.next();

    if(query.isValid())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("Employee already exists."));
        return true;
    }
    return false;
}

void EmployeeDialog::addEmployee()
{
    QSqlRecord record;

    QSqlField id("id", QVariant::Int);
    QSqlField firstName("first_name", QVariant::String);
    QSqlField lastName("last_name", QVariant::String);
    QSqlField fullName("full_name", QVariant::String);
    QSqlField gender("gender", QVariant::String);
    QSqlField countryId("country_id", QVariant::Int);
    QSqlField occupationId("occupation_id", QVariant::Int);
    QSqlField civilIdNumber("civil_id_number", QVariant::String);
    QSqlField firstPhoneNumber("first_phone_number", QVariant::String);
    QSqlField secondPhoneNumber("second_phone_number", QVariant::String);
    QSqlField email("email", QVariant::String);
    QSqlField createdAt("created_at", QVariant::DateTime);
    QSqlField updatedAt("updated_at", QVariant::DateTime);

    QVariant countryIdFromBox = getCountryId(countryComboBox->currentIndex());
    QVariant occupationIdFromBox = getOccupationId(occupationComboBox->currentIndex());

    id.setAutoValue(true);
    firstName.setValue(QVariant(firstNameLineEdit->text()));
    lastName.setValue(QVariant(lastNameLineEdit->text()));
    fullName.setValue(QVariant(firstNameLineEdit->text() + " " + lastNameLineEdit->text()));
    gender.setValue(QVariant(genderComboBox->currentText()));
    countryId.setValue(countryIdFromBox);
    occupationId.setValue(occupationIdFromBox);
    civilIdNumber.setValue(QVariant(civilIdLineEdit->text()));
    firstPhoneNumber.setValue(QVariant(firstPhoneNumberLineEdit->text()));
    secondPhoneNumber.setValue(QVariant(secondPhoneNumberLineEdit->text()));
    email.setValue(QVariant(emailLineEdit->text()));
    createdAt.setValue(QVariant(QDateTime::currentDateTime()));
    updatedAt.setValue(QVariant(QDateTime::currentDateTime()));

    record.append(id);
    record.append(firstName);
    record.append(lastName);
    record.append(fullName);
    record.append(gender);
    record.append(occupationId);
    record.append(countryId);
    record.append(civilIdNumber);
    record.append(firstPhoneNumber);
    record.append(secondPhoneNumber);
    record.append(email);
    record.append(createdAt);
    record.append(updatedAt);

    if(isFieldInputValid()
        && !employeeExists()
        && employeeModel->insertRecord(-1, record))
    {
        accept();
    }
    else
    {
        employeeModel->revertAll();
    }
}

void EmployeeDialog::editEmployee()
{
    employeeRecord.setValue("first_name", QVariant(firstNameLineEdit->text()));
    employeeRecord.setValue("last_name", QVariant(lastNameLineEdit->text()));
    employeeRecord.setValue("full_name", QVariant(firstNameLineEdit->text() + " " + lastNameLineEdit->text()));
    employeeRecord.setValue("gender", QVariant(genderComboBox->currentText()));
    employeeRecord.setValue("civil_id_number", QVariant(civilIdLineEdit->text()));
    employeeRecord.setValue("first_phone_number", QVariant(firstPhoneNumberLineEdit->text()));
    employeeRecord.setValue("second_phone_number", QVariant(secondPhoneNumberLineEdit->text()));
    employeeRecord.setValue("email", QVariant(emailLineEdit->text()));
    employeeRecord.setValue("updated_at", QVariant(QDateTime::currentDateTime()));

    /* Setting country_id */
    QSqlField countryId("country_id", QVariant::Int);
    QVariant countryIdFromBox = getCountryId(countryComboBox->currentIndex());
    countryId.setValue(countryIdFromBox);
    employeeRecord.append(countryId);
    employeeRecord.setValue("country_id", countryIdFromBox);

    /* Setting occupation_id */
    QSqlField occupationId("occupation_id", QVariant::Int);
    QVariant occupationIdFromBox = getOccupationId(occupationComboBox->currentIndex());
    occupationId.setValue(occupationIdFromBox);
    employeeRecord.append(occupationId);
    employeeRecord.setValue("occupation_id", occupationIdFromBox);

    if(isFieldInputValid() && employeeModel->setRecord(employeeRow, employeeRecord))
    {
        employeeModel->submitAll();
        accept();
    }
    else
    {
        employeeModel->revertAll();
    }
}

QVariant EmployeeDialog::getCountryId(int index)
{

    QVariant countryIdFromBox = countryModel->record(index)
                                .field("id").value();
    return countryIdFromBox;
}

QVariant EmployeeDialog::getOccupationId(int index)
{
    QVariant occupationIdFromBox = occupationModel->record(index)
                                   .field("id").value();
    return occupationIdFromBox;
}
