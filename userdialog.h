#ifndef USERDIALOG_H
#define USERDIALOG_H

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
#include <QUuid>
#include <QCryptographicHash>
#include <QLabel>

class UserDialog : public QDialog
{
    Q_OBJECT
public:
    UserDialog(QSqlRelationalTableModel* model, QWidget *parent = 0);
    UserDialog(QSqlRelationalTableModel *model, QSqlRecord &record, int row, QWidget *parent = 0);

private slots:
    void addUser();
    void editUser();

private:
    void initSetup();
    void setupLayout();
    void setupUserModel();
    int getRoleId() const;
    bool isFieldInputValid();
    bool userExists();
    bool isPasswordEmpty();
    void fillFieldInputs();

    QSqlRelationalTableModel *userModel;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

    QLineEdit *usernameLineEdit;
    QLineEdit *emailLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *passwordConfirmationLineEdit;

    QLabel *passwordLabel;
    QLabel *passwordConfirmationLabel;

    QComboBox *rolesCombobox;
    QSqlTableModel *roleModel;

    QSqlRecord userRecord;
    int userRow;
};

#endif // USERDIALOG_H
