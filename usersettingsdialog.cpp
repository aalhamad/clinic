#include "usersettingsdialog.h"

UserSettingsDialog::UserSettingsDialog(User& user, QWidget *parent)
                   :QDialog(parent), m_user(user)
{
    setupLayout();
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(updateUser()));
}

void UserSettingsDialog::setupLayout()
{
    usernameLineEdit = new QLineEdit(m_user.username());
    usernameLineEdit->setEnabled(false);

    emailLineEdit = new QLineEdit;
    emailLineEdit->setText(m_user.email());

    oldPasswordLineEdit = new QLineEdit;
    newPasswordLineEdit = new QLineEdit;
    passwordConfirmationLineEdit = new QLineEdit;

    okButton = new QPushButton(tr("&OK"));
    cancelButton = new QPushButton(tr("&Cancel"));
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(okButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Username: "), usernameLineEdit);
    formLayout->addRow(tr("Email"), emailLineEdit);
    formLayout->addRow(tr("Old Password:"), oldPasswordLineEdit);
    formLayout->addRow(tr("New Password: "), newPasswordLineEdit);
    formLayout->addRow(tr("Password Confirmation: "), passwordConfirmationLineEdit);
    formLayout->addWidget(buttonBox);
    setLayout(formLayout);
}

void UserSettingsDialog::updateUser()
{
    QSqlQuery query;

    query.prepare("UPDATE users SET email = :email, hashed_password = :hashed_password, salt = :salt WHERE id = :id");
    query.bindValue(":email", emailLineEdit->text());
    query.bindValue(":id", m_user.id());

    if(newPasswordLineEdit->text().isEmpty())
    {
        query.bindValue(":hashed_password", m_user.hashedPassword());
        query.bindValue(":salt", m_user.salt());
        query.exec();
        accept();
    }

    if(!newPasswordLineEdit->text().isEmpty())
    {
        if(newPasswordLineEdit->text() != passwordConfirmationLineEdit->text())
        {
            QMessageBox::information(this, tr("Input Validation"),
                                     tr("New password doesn't match password confirmation"));
            return;
        }

        bool okOldPass = User::authenticate(m_user.username(), oldPasswordLineEdit->text());
        if(!okOldPass)
        {
            QMessageBox::information(this, tr("Input Validation"),
                                     tr("Wrong old password."));
            return;
        }

        QString salt = QUuid::createUuid().toString();
        QString hashed_password = QCryptographicHash::hash(newPasswordLineEdit->text().toAscii()
                                                           + salt.toAscii(),
                                                           QCryptographicHash::Sha1);
        query.bindValue(":hashed_password", hashed_password);
        query.bindValue(":salt", salt);
        query.exec();
        accept();
    }
}


