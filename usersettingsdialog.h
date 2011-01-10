#ifndef USERSETTINGSDIALOG_H
#define USERSETTINGSDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLineEdit>
#include <QUuid>
#include <QCryptographicHash>
#include <QMessageBox>
#include "user.h"

class UserSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserSettingsDialog(User& user, QWidget *parent = 0);

private slots:
    void updateUser();

private:
    void setupLayout();

    QLineEdit *usernameLineEdit;
    QLineEdit *newPasswordLineEdit;
    QLineEdit *oldPasswordLineEdit;
    QLineEdit *passwordConfirmationLineEdit;
    QLineEdit *emailLineEdit;

    QPushButton *okButton;
    QPushButton *cancelButton;

    QDialogButtonBox *buttonBox;

    User m_user;
};

#endif // USERSETTINGSDIALOG_H
