#include "admindbwizardpage.h"

AdminDbWizardPage::AdminDbWizardPage(QWidget *parent) :
    QWizardPage(parent)
{
    setupLayout();
    setupRegisterFields();
}

void AdminDbWizardPage::setupLayout()
{
   userNameLineEdit = new QLineEdit;
   emailLineEdit = new QLineEdit;
   passwordLineEdit = new QLineEdit;
   passwordConfirmationLineEdit = new QLineEdit;
   passwordLineEdit->setEchoMode(QLineEdit::Password);
   passwordConfirmationLineEdit->setEchoMode(QLineEdit::Password);

   QFormLayout *formLayout = new QFormLayout;
   QString asterisk = "<i style=color:red>*</i>";
   formLayout->addRow(tr("&Username: ") + asterisk, userNameLineEdit);
   formLayout->addRow(tr("Password: ")+ asterisk, passwordLineEdit);
   formLayout->addRow(tr("Password Confirmation: ") + asterisk, passwordConfirmationLineEdit);
   formLayout->addRow(tr("&Email"), emailLineEdit);
   setLayout(formLayout);

   setTitle(tr("Administration"));
   setSubTitle(tr("Create a unique username and password for the use of the admistration."));
}

void AdminDbWizardPage::setupRegisterFields()
{
    registerField("username*", userNameLineEdit);
    registerField("password*", passwordLineEdit);
    registerField("passwordConfirmation*", passwordConfirmationLineEdit);
    registerField("email", emailLineEdit);
}

bool AdminDbWizardPage::validatePage()
{
    if(userNameLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("Username can't be empty"));
        return false;
    }

    if(passwordConfirmationLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("Password can't be empty"));
        return false;
    }

    if(passwordLineEdit->text() != passwordConfirmationLineEdit->text())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("Password don't match password confirmation"));
        return false;
    }
    return true;
}
