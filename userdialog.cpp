#include "userdialog.h"

UserDialog::UserDialog(QSqlRelationalTableModel *model, QWidget *parent)
           :QDialog(parent), userModel(model)
{
    initSetup();
    connect(okButton, SIGNAL(clicked()), this, SLOT(addUser()));
}

UserDialog::UserDialog(QSqlRelationalTableModel *model,
                       QSqlRecord &record, int row, QWidget *parent)
           :QDialog(parent), userModel(model), userRecord(record), userRow(row)
{
    initSetup();
    fillFieldInputs();
    connect(okButton, SIGNAL(clicked()), this, SLOT(editUser()));
}

void UserDialog::initSetup()
{
    setupLayout();
    setupUserModel();
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

void UserDialog::setupLayout()
{
    okButton = new QPushButton(tr("&OK"));
    cancelButton = new QPushButton(tr("&Cancel"));
    buttonBox = new QDialogButtonBox;

    buttonBox->addButton(okButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

    usernameLineEdit = new QLineEdit;
    emailLineEdit = new QLineEdit;
    passwordLineEdit = new QLineEdit;
    passwordConfirmationLineEdit = new QLineEdit;

    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordConfirmationLineEdit->setEchoMode(QLineEdit::Password);

    rolesCombobox = new QComboBox;

    QString asterisk = "<i style=color:red>*</i>";
    passwordLabel = new QLabel(tr("Password: ") + asterisk);
    passwordConfirmationLabel = new QLabel(tr("Password Confirmation: ") + asterisk);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("&Username: ") + asterisk, usernameLineEdit);
    formLayout->addRow(tr("&Email:"), emailLineEdit);
    formLayout->addRow(passwordLabel, passwordLineEdit);
    formLayout->addRow(passwordConfirmationLabel, passwordConfirmationLineEdit);
    formLayout->addRow(tr("&Role: "), rolesCombobox);
    formLayout->addWidget(buttonBox);

    setLayout(formLayout);
}

void UserDialog::setupUserModel()
{
    QSqlDatabase db = userModel->database();

    roleModel = new QSqlTableModel(this, db);
    roleModel->setTable("roles");
    roleModel->select();

    rolesCombobox->clear();
    for(int i = 0; i < roleModel->rowCount(); i++)
    {
        rolesCombobox->addItem(roleModel->record(i).field("type").value().toString(),
                               roleModel->record(i).field("id").value().toInt());
    }

    rolesCombobox->setCurrentIndex(2);
}

void UserDialog::addUser()
{
    QSqlRecord record;

    QSqlField id("id", QVariant::Int);
    QSqlField username("username", QVariant::String);
    QSqlField email("email", QVariant::String);
    QSqlField hashedPassword("hashed_password", QVariant::String);
    QSqlField salt("salt", QVariant::String);
    QSqlField roleId("role_id", QVariant::Int);

    id.setAutoValue(true);
    username.setValue(QVariant(usernameLineEdit->text()));
    email.setValue(QVariant(emailLineEdit->text()));

    QString generatedSalt = QUuid::createUuid().toString();
    QString generatedHashedPassword = QCryptographicHash::hash(passwordLineEdit->text().toAscii()
                                                               + generatedSalt.toAscii(),
                                                               QCryptographicHash::Sha1);

    hashedPassword.setValue(QVariant(generatedHashedPassword));
    salt.setValue(QVariant(generatedSalt));
    roleId.setValue(QVariant(getRoleId()));

    record.append(id);
    record.append(username);
    record.append(email);
    record.append(hashedPassword);
    record.append(salt);
    record.append(roleId);

    if(isFieldInputValid()
        && !isPasswordEmpty()
        && !userExists()
        && userModel->insertRecord(-1, record))
    {
        accept();
    }
    else
    {
        userModel->revertAll();
    }
}

void UserDialog::fillFieldInputs()
{
    usernameLineEdit->setText(userRecord.field("username").value().toString());
    emailLineEdit->setText(userRecord.field("email").value().toString());

    passwordLabel->setText(tr("New Password:"));
    passwordConfirmationLabel->setText(tr("Password Confirmation: "));

    rolesCombobox->setCurrentIndex(rolesCombobox->findText(userRecord
                                                           .field("type")
                                                           .value().toString()));
}

bool UserDialog::userExists()
{
    QSqlQuery query(userModel->database());
    query.prepare("SELECT username FROM users WHERE username = :username");
    query.bindValue(":username", usernameLineEdit->text());
    query.exec();
    query.next();

    if(query.isValid())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("User number already exists."));
        return true;
    }
    return false;
}

void UserDialog::editUser()
{
    userRecord.setValue("username", QVariant( usernameLineEdit->text()));
    userRecord.setValue("email", QVariant(emailLineEdit->text()));
    userRecord.setValue("role_id", QVariant(getRoleId()));

    if(passwordLineEdit->text().isEmpty())
    {
        userRecord.setValue("salt", userRecord.field("salt").value());
        userRecord.setValue("hashed_password", userRecord.field("hashed_password").value());
    }
    else
    {
        QString generatedSalt = QUuid::createUuid().toString();
        QString generatedHashedPassword = QCryptographicHash::hash(passwordLineEdit->text().toAscii()
                                                                   + generatedSalt.toAscii(),
                                                                   QCryptographicHash::Sha1);
        userRecord.setValue("salt", QVariant(generatedSalt));
        userRecord.setValue("hashed_password", QVariant(generatedHashedPassword));
    }

    QSqlField roleId("role_id", QVariant::Int);
    roleId.setValue(QVariant(getRoleId()));
    userRecord.append(roleId);

    if(isFieldInputValid() && userModel->setRecord(userRow, userRecord))
    {
        userModel->submitAll();
        accept();
    }
    else
    {
        userModel->revertAll();
    }

}

int UserDialog::getRoleId() const
{
    int id = rolesCombobox->itemData(rolesCombobox->currentIndex()).toInt();
    return id;
}

bool UserDialog::isFieldInputValid()
{
    if(passwordLineEdit->text() != passwordConfirmationLineEdit->text())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("Password don't match password confirmation"));
        return false;
    }

    if(usernameLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("Username can't be empty."));
        return false;
    }

    return true;
}

bool UserDialog::isPasswordEmpty()
{
    if(passwordLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("Input Validation"),
                                 tr("Password can't be empty"));
        return true;
    }

    return false;
}
