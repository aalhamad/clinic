#ifndef EMPLOYEEDIALOG_H
#define EMPLOYEEDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlRecord>
#include <QFormLayout>
#include <QSqlField>
#include <QMessageBox>
#include <QSqlError>
#include <QDateTime>
#include <QSqlQuery>

class EmployeeDialog : public QDialog
{
    Q_OBJECT
public:
    EmployeeDialog(QSqlRelationalTableModel *model, QWidget *parent = 0);
    EmployeeDialog(QSqlRelationalTableModel *model,
                  QSqlRecord &record, int row, QWidget *parent = 0);

private slots:
    void addEmployee();
    void editEmployee();

private:
    void initSetup();
    void setupLayout();
    void setupEmployeeModel();
    bool isFieldInputValid();
    bool employeeExists();
    void fillFieldInputs();

    QVariant getCountryId(int index);
    QVariant getOccupationId(int index);

    QLineEdit *firstNameLineEdit;
    QLineEdit *lastNameLineEdit;
    QComboBox *genderComboBox;
    QComboBox *occupationComboBox;
    QComboBox *countryComboBox;
    QLineEdit *civilIdLineEdit;
    QLineEdit *firstPhoneNumberLineEdit;
    QLineEdit *secondPhoneNumberLineEdit;
    QLineEdit *emailLineEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;
    QSqlRelationalTableModel *employeeModel;
    QSqlTableModel *countryModel;
    QSqlTableModel *occupationModel;
    QSqlRecord employeeRecord;
    int employeeRow;

    enum { Male, Female };

};

#endif // EMPLOYEEDIALOG_H
