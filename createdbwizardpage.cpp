#include "createdbwizardpage.h"

CreateDbWizardPage::CreateDbWizardPage(QWidget *parent) :
    QWizardPage(parent)
{
    setupLayout();
    connect(changeToolButton, SIGNAL(clicked()), this, SLOT(showOpenFile()));
    connect(createDbCheckBox, SIGNAL(clicked(bool)), this, SLOT(onCreateCheckBoxClicked(bool)));
    setupRegisterFields();
}

void CreateDbWizardPage::setupLayout()
{
    fileNameFileEdit = new QLineEdit;
    fileNameLabel = new QLabel(tr("&File Name: "));
    fileNameLabel->setBuddy(fileNameFileEdit);
    fileNameFileEdit->setText(QCoreApplication::applicationDirPath() + "/" + "production.db");

    createDbCheckBox = new QCheckBox(tr("database exist?"));
    changeToolButton = new QToolButton;
    changeToolButton->setText(tr("..."));
    changeToolButton->setToolTip(tr("Change file name or path"));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(fileNameLabel, 0, 0);
    mainLayout->addWidget(fileNameFileEdit, 0, 1);
    mainLayout->addWidget(changeToolButton, 0, 2);
    mainLayout->addWidget(createDbCheckBox, 1, 1);
    setLayout(mainLayout);

    setTitle(tr("Setup Database"));
    setSubTitle(tr("Create a database or set the path of an exisiting database. "
                   "Make sure the database exist if you are setting its path."));
}


void CreateDbWizardPage::showOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Database file"),
                                                    ".", tr("Database File (*.db)"));

    if(!fileName.isEmpty())
    {
        fileNameFileEdit->setText(fileName);
    }
}


void CreateDbWizardPage::setupRegisterFields()
{
    registerField("fileName", fileNameFileEdit);
    registerField("cbCreateDb", createDbCheckBox);
}

void CreateDbWizardPage::onCreateCheckBoxClicked(bool final)
{
    setFinalPage(final);
}

int CreateDbWizardPage::nextId() const
{
    if(createDbCheckBox->isChecked())
    {
        return -1;
    }
    else
    {
        return 2;
    }
}

bool CreateDbWizardPage::validatePage()
{
    bool fileExists = QFile::exists(fileNameFileEdit->text());

    if(!fileExists && createDbCheckBox->isChecked())
    {
        QMessageBox::information(this, tr("File validation"),
                                 tr("File: %1 don't not exist.").arg(fileNameFileEdit->text()));
        return false;
    }

    return true;
}
