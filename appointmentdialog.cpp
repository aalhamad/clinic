#include "appointmentdialog.h"

AppointmentDialog::AppointmentDialog(QSqlRelationalTableModel *model, QWidget *parent)
                  : QDialog(parent), appointmentModel(model)
{
    initSetup();
    connect(okButton, SIGNAL(clicked()), this, SLOT(addAppointment()));

}

AppointmentDialog::AppointmentDialog(QSqlRelationalTableModel *model,
                                     QSqlRecord &record, int row, QWidget *parent)
                  :QDialog(parent), appointmentModel(model), appointmentRecord(record), appointmentRow(row)
{
    initSetup();
    fillFieldInputs();
    connect(okButton, SIGNAL(clicked()), this, SLOT(editAppointment()));
}

void AppointmentDialog::initSetup()
{
    setupLayout();
    setupAppointmentModel();
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

void AppointmentDialog::setupLayout()
{
    startDateEdit  = new QDateEdit;
    endDateEdit = new QDateEdit;
    startDateEdit->setDateTime(QDateTime::currentDateTime());
    endDateEdit->setDateTime(QDateTime::currentDateTime());
    startDateEdit->setDisplayFormat("MMMM dd yyyy");
    endDateEdit->setDisplayFormat("MMMM dd yyyy");
    startDateEdit->setCalendarPopup(true);
    endDateEdit->setCalendarPopup(true);

    startTimeEdit = new QTimeEdit;
    startTimeEdit->setDateTime(QDateTime::currentDateTime());
    endTimeEdit = new QTimeEdit;
    endTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(60*30));

    patientComboBox = new QComboBox;
    employeeComboBox = new QComboBox;
    visitComboBox = new QComboBox;

    okButton = new QPushButton(tr("&OK"));
    cancelButton = new QPushButton(tr("&Cancel"));
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(okButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Start Date:"), startDateEdit);
    formLayout->addRow(tr("Start Time:"), startTimeEdit);
    formLayout->addRow(tr("End Date:"), endDateEdit);
    formLayout->addRow(tr("End Time:"), endTimeEdit);
    formLayout->addRow(tr("Patient:"), patientComboBox);
    formLayout->addRow(tr("Employee:"), employeeComboBox);
    formLayout->addRow(tr("Visit Type: "), visitComboBox);
    formLayout->addWidget(buttonBox);

    setLayout(formLayout);
}

void AppointmentDialog::setupAppointmentModel()
{
    QSqlDatabase db = appointmentModel->database();
    patientModel = new QSqlTableModel(this, db);
    patientModel->setTable("patients");
    patientModel->select();
    patientComboBox->clear();
    for(int i = 0; i < patientModel->rowCount(); i++)
    {
        patientComboBox->addItem(patientModel->record(i).field("full_name").value().toString(),
                                 patientModel->record(i).field("id").value());
    }

    employeeModel = new QSqlTableModel(this, db);
    employeeModel->setTable("employees");
    employeeModel->select();
    employeeComboBox->clear();
    for(int i = 0; i < employeeModel->rowCount(); i++)
    {
        employeeComboBox->addItem(employeeModel->record(i).field("full_name").value().toString(),
                                  employeeModel->record(i).field("id").value());
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

void AppointmentDialog::fillFieldInputs()
{
    startDateEdit->setDate(appointmentRecord.field("start").value().toDateTime().date());
    startTimeEdit->setTime(appointmentRecord.field("start").value().toDateTime().time());
    endDateEdit->setDate(appointmentRecord.field("end").value().toDateTime().date());
    endTimeEdit->setTime(appointmentRecord.field("end").value().toDateTime().time());

    patientComboBox->setCurrentIndex(patientComboBox->findText(appointmentRecord
                                                               .field("patients_full_name_2")
                                                               .value().toString()));
    employeeComboBox->setCurrentIndex(employeeComboBox->findText(appointmentRecord
                                                                 .field("full_name")
                                                                 .value().toString()));
    visitComboBox->setCurrentIndex(visitComboBox->findText(appointmentRecord
                                                           .field("type")
                                                           .value().toString()));
}

void AppointmentDialog::addAppointment()
{
    QSqlRecord record;

    QSqlField id("id", QVariant::Int);
    QSqlField start("start", QVariant::DateTime);
    QSqlField end("end", QVariant::DateTime);
    QSqlField employeeId("employee_id", QVariant::Int);
    QSqlField patientId("patient_id", QVariant::Int);
    QSqlField visitId("visit_id", QVariant::Int);

    id.setAutoValue(true);
    start.setValue(QVariant(getStart()));
    end.setValue(QVariant(getEnd()));

    patientId.setValue(QVariant(getPatientId()));
    employeeId.setValue(QVariant(getEmployeeId()));
    visitId.setValue(QVariant(getVisitId()));

    record.append(id);
    record.append(start);
    record.append(end);
    record.append(patientId);
    record.append(employeeId);
    record.append(visitId);

    if(isFieldInputValid() && appointmentModel->insertRecord(-1, record))
    {
        accept();
    }
    else
    {
        appointmentModel->revertAll();
    }
}

bool AppointmentDialog::isFieldInputValid()
{
    if(startDateEdit->date() > endDateEdit->date())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("Start date must be less then End date"));
        return false;
    }

    if(startTimeEdit->time() > endTimeEdit->time())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("Start time must be less then End time"));
        return false;
    }

    if(patientComboBox->count() <= 0)
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("There are no patients. Please add some patients."));
        return false;
    }

    if(employeeComboBox->count() <= 0)
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("There are no employees. Please add some employees"));
        return false;
    }


    return true;
}

void AppointmentDialog::editAppointment()
{
    appointmentRecord.setValue("start", QVariant(getStart()));
    appointmentRecord.setValue("end", QVariant(getEnd()));

    QSqlField patientId("patient_id", QVariant::Int);
    patientId.setValue(QVariant(getPatientId()));

    QSqlField employeeId("employee_id", QVariant::Int);
    employeeId.setValue(QVariant(getEmployeeId()));

    QSqlField visitId("visit_id", QVariant::Int);
    visitId.setValue(QVariant(getVisitId()));

    appointmentRecord.append(patientId);
    appointmentRecord.append(employeeId);
    appointmentRecord.append(visitId);

    if(isFieldInputValid() &&
       appointmentModel->setRecord(appointmentRow, appointmentRecord))
    {
        appointmentModel->submitAll();
        accept();
    }
    else
    {
        appointmentModel->revertAll();
    }
}

int AppointmentDialog::getPatientId() const
{
   int id = patientComboBox->itemData(patientComboBox->currentIndex()).toInt();
   return id;
}

int AppointmentDialog::getEmployeeId() const
{
    int id = employeeComboBox->itemData(employeeComboBox->currentIndex()).toInt();
    return id;
}

int AppointmentDialog::getVisitId() const
{
    int id = visitComboBox->itemData(visitComboBox->currentIndex()).toInt();
    return id;
}

QDateTime AppointmentDialog::getStart() const
{
    QDateTime startDateTime;
    startDateTime.setDate(startDateEdit->date());
    startDateTime.setTime(startTimeEdit->time());
    return startDateTime;
}

QDateTime AppointmentDialog::getEnd() const
{
    QDateTime endDateTime;
    endDateTime.setDate(endDateEdit->date());
    endDateTime.setTime(endTimeEdit->time());
    return endDateTime;
}
