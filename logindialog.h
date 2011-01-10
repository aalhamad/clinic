#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QDebug>

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = 0);
    QString username() const;
    QString password() const;

private:
    void setupLayout();
    QDialogButtonBox *buttonBox;
    QPushButton *loginButton;
    QPushButton *cancelButton;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
};

#endif // LOGINDIALOG_H
