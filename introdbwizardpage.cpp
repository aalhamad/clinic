#include "introdbwizardpage.h"

IntroDbWizardPage::IntroDbWizardPage(QWidget *parent) :
    QWizardPage(parent)
{
    setupLayout();
    setTitle(tr("Introduction"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));
}

void IntroDbWizardPage::setupLayout()
{
    label = new QLabel(tr("Welcome! This wizard will help you setup your database\n"
                          "and adminstration privileges for %1 application.\n\n"
                          "This wizard will show up again only once your database has been\n "
                          "successfuly setup.\n\n"
                          "It is recommened that you close all other appplications \n"
                          "before setting up the database.\n\n"
                          "Click Next to continue.")
                       .arg(QCoreApplication::applicationName()));
    label->setWordWrap(true);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(label);
    setLayout(mainLayout);
}
