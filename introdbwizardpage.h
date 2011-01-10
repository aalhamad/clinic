#ifndef INTRODBWIZARDPAGE_H
#define INTRODBWIZARDPAGE_H

#include <QWizardPage>
#include <QLabel>
#include <QHBoxLayout>
#include <QCoreApplication>

class IntroDbWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit IntroDbWizardPage(QWidget *parent = 0);

private:
    void setupLayout();
    QLabel *label;

};

#endif // INTRODBWIZARDPAGE_H
