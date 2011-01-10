#ifndef ADMINDBWIZARDPAGE_H
#define ADMINDBWIZARDPAGE_H

#include <QWizardPage>
#include <QLineEdit>
#include <QFormLayout>
#include <QMessageBox>

class AdminDbWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit AdminDbWizardPage(QWidget *parent = 0);
    bool validatePage();

private:
    void setupLayout();
    void setupRegisterFields();
    QLineEdit *userNameLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *passwordConfirmationLineEdit;
    QLineEdit *emailLineEdit;
};

#endif // ADMINDBWIZARDPAGE_H
