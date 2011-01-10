#ifndef APPOINTMENTDIALOG_H
#define APPOINTMENTDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QComboBox>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlRecord>
#include <QDateEdit>
#include <QTimeEdit>
#include <QDateTime>
#include <QFormLayout>
#include <QSqlField>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

class AppointmentDialog : public QDialog
{
    Q_OBJECT
public:
    AppointmentDialog(QSqlRelationalTableModel *model, QWidget *parent = 0);
    AppointmentDialog(QSqlRelationalTableModel *model, QSqlRecord &record, int row, QWidget *parent = 0);

private slots:
    void addAppointment();
    void editAppointment();

private:
    void initSetup();
    void setupLayout();
    void setupAppointmentModel();
    bool isFieldInputValid();
    void fillFieldInputs();

    int getPatientId() const;
    int getEmployeeId() const;
    int getVisitId() const;
    QDateTime getStart() const;
    QDateTime getEnd() const;

    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QTimeEdit *startTimeEdit;
    QTimeEdit *endTimeEdit;

    QComboBox *employeeComboBox;
    QComboBox *visitComboBox;
    QComboBox *patientComboBox;

    QPushButton *okButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;
    QSqlRelationalTableModel *appointmentModel;
    QSqlTableModel *patientModel;
    QSqlTableModel *employeeModel;
    QSqlTableModel *visitModel;
    QSqlRecord appointmentRecord;
    int appointmentRow;

};

#endif // APPOINTMENTDIALOG_H
