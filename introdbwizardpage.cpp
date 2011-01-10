#include "introdbwizardpage.h"

IntroDbWizardPage::IntroDbWizardPage(QWidget *parent) :
    QWizardPage(parent)
{
    setupLayout();
    setTitle(tr("Introduction"));
}

void IntroDbWizardPage::setupLayout()
{
    label = new QLabel(tr("This wizard will help you setup your database "
                          "and adminstration privileges for %1.").arg(QCoreApplication::applicationName()));
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(label);
    setLayout(mainLayout);
}
