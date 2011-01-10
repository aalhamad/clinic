#ifndef CREATEDBWIZARDPAGE_H
#define CREATEDBWIZARDPAGE_H

#include <QWizardPage>
#include <QGridLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QToolButton>
#include <QCoreApplication>
#include <QFileDialog>
#include <QMessageBox>

class CreateDbWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit CreateDbWizardPage(QWidget *parent = 0);
    bool validatePage();
    int nextId() const;

private slots:
    void showOpenFile();
    void onCreateCheckBoxClicked(bool final);


private:
    void setupLayout();
    void setupRegisterFields();
    QLineEdit *fileNameFileEdit;
    QLabel *fileNameLabel;
    QCheckBox *createDbCheckBox;
    QToolButton *changeToolButton;
};

#endif // CREATEDBWIZARDPAGE_H
