#include "logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent)
{
    setupLayout();
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(loginButton, SIGNAL(clicked()), this, SLOT(accept()));
}

void LoginDialog::setupLayout()
{
    loginButton = new QPushButton(tr("Login"));
    cancelButton = new QPushButton(tr("Cancel"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(loginButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

    usernameLineEdit = new QLineEdit;
    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Username: "), usernameLineEdit);
    formLayout->addRow(tr("Password: "), passwordLineEdit);
    formLayout->addWidget(buttonBox);

    setLayout(formLayout);
}

QString LoginDialog::username() const
{
    return usernameLineEdit->text();
}

QString LoginDialog::password() const
{
    return passwordLineEdit->text();
}
