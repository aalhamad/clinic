#ifndef PATIENTDIALOG_H
#define PATIENTDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QFormLayout>
#include <QSqlField>
#include <QMessageBox>
#include <QSqlError>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

class PatientDialog : public QDialog
{
    Q_OBJECT
public:
    PatientDialog(QSqlRelationalTableModel *model, QWidget *parent = 0);
    PatientDialog(QSqlRelationalTableModel *model, QSqlRecord &record, int row, QWidget *parent = 0);

private slots:
    void addPatient();
    void editPatient();

private:
    void initSetup();
    void setupLayout();
    void setupPatientModel();
    bool isFieldInputValid();
    bool patientExists();
    bool fileNumberExists();
    void fillFieldInputs();
    QLineEdit *fileNumberLineEdit;
    QLineEdit *firstNameLineEdit;
    QLineEdit *lastNameLineEdit;
    QComboBox *genderComboBox;
    QComboBox *countryComboBox;
    QLineEdit *civilIdLineEdit;
    QLineEdit *firstPhoneNumberLineEdit;
    QLineEdit *secondPhoneNumberLineEdit;
    QLineEdit *emailLineEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;
    QSqlRelationalTableModel *patientModel;
    QSqlTableModel *countryModel;
    QSqlRecord patientRecord;
    int patientRow;

    enum { Male, Female };
};

#endif // PATIENTDIALOG_H
