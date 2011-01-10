#include "adminwidget.h"

AdminWidget::AdminWidget(QSqlDatabase& database,
                           QStatusBar *statusBar, QWidget *parent)
                           :QWidget(parent), mainWindowStatusBar(statusBar), m_db(database)
{
    setupLayout();
    setupModels();
    setupUserTableView();

    connect(applyButton, SIGNAL(clicked()), this, SLOT(onApplyButtonClicked()));
    connect(changeFileNameToolButton, SIGNAL(clicked()), this, SLOT(onChangeFileNameToolButtonClicked()));
    connect(revertButton, SIGNAL(clicked()), this, SLOT(onRevertButtonClicked()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addEntry()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editEntry()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteEntry()));
    connect(userTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editEntry()));
}

void AdminWidget::setupLayout()
{
    QLabel *fileNamelabel = new QLabel(tr("&File Name: "));
    fileNameLineEdit = new QLineEdit;
    fileNamelabel->setBuddy(fileNameLineEdit);
    QSettings settings(Globals::OrganizationName, Globals::ApplicationName);
    QString fileName = settings.value(Globals::FileDbDestination, "").toString();

    fileNameLineEdit->setText(fileName);

    changeFileNameToolButton = new QToolButton;
    changeFileNameToolButton->setText(tr("..."));
    changeFileNameToolButton->setToolTip(tr("Change the file name or path"));

    QLabel *usersLabel = new QLabel(tr("Users"));
    userTableView = new QTableView;

    applyButton = new QPushButton(tr("Apply"));
    applyButton->setToolTip(tr("Apply changes for database file name and directroy"));
    revertButton = new QPushButton(tr("Revert"));

    addButton = new QPushButton(tr("&Add"));
    editButton = new QPushButton(tr("&Edit"));
    deleteButton = new QPushButton(tr("Delete"));


    QHBoxLayout *fileButtonsLayout = new QHBoxLayout;
    fileButtonsLayout->addWidget(revertButton);
    fileButtonsLayout->addWidget(applyButton);

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addWidget(fileNamelabel, 0, 0);
    topLayout->addWidget(fileNameLineEdit, 0, 1);
    topLayout->addWidget(changeFileNameToolButton, 0, 2);
    topLayout->addLayout(fileButtonsLayout, 1, 1, Qt::AlignRight);

    QHBoxLayout *userButtonLayout = new QHBoxLayout;
    userButtonLayout->addStretch();
    userButtonLayout->addWidget(addButton);
    userButtonLayout->addWidget(editButton);
    userButtonLayout->addWidget(deleteButton);

    QVBoxLayout *bottomLayout = new QVBoxLayout;
    bottomLayout->addWidget(usersLabel);
    bottomLayout->addWidget(userTableView);
    bottomLayout->addLayout(userButtonLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);
}

void AdminWidget::updateTableView()
{
    setupModels();
}

void AdminWidget::setupModels()
{
    userModel = new QSqlRelationalTableModel(this, m_db);
    userModel->setTable("users");
    userModel->setRelation(Role, QSqlRelation("roles", "id", "type"));
    userModel->setHeaderData(Username, Qt::Horizontal, tr("Username"));
    userModel->setHeaderData(Email, Qt::Horizontal, tr("Email"));
    userModel->setHeaderData(Role, Qt::Horizontal, tr("Role"));
    userModel->select();
}

void AdminWidget::setupUserTableView()
{
    userTableView->setModel(userModel);
    userTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    userTableView->setColumnHidden(Id, true);
    userTableView->setColumnHidden(HashedPassword, true);
    userTableView->setColumnHidden(Salt, true);
    userTableView->horizontalHeader()->setStretchLastSection(true);
    userTableView->resizeColumnsToContents();
}

void AdminWidget::onApplyButtonClicked()
{
    QSettings settings(Globals::OrganizationName, Globals::ApplicationName);
    QString oldFileName = settings.value(Globals::FileDbDestination, "").toString();

    bool fileExists = QFile::exists(fileNameLineEdit->text());

    if(fileExists)
    {
        settings.setValue(Globals::FileDbDestination, fileNameLineEdit->text());
        mainWindowStatusBar->showMessage(tr("File name and directory successfully changed."));
    }
    else
    {
        QMessageBox::information(this, tr("File Validation"),
                                 tr("File: %1 doesn't exist.").arg(fileNameLineEdit->text()));
    }


    QString newFileName = settings.value(Globals::FileDbDestination, "").toString();

    if(oldFileName != newFileName)
    {
        QMessageBox::information(this, tr("Restart"),
                                 tr("You need to retart the application for the changes to take places"));
    }
}

void AdminWidget::onChangeFileNameToolButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Database file"),
                                                    ".", tr("Database File (*.db)"));
    if(!fileName.isEmpty())
    {
        fileNameLineEdit->setText(fileName);
    }

}

void AdminWidget::onRevertButtonClicked()
{
    QSettings settings(Globals::OrganizationName, Globals::ApplicationName);
    QString fileName = settings.value(Globals::FileDbDestination, "").toString();
    fileNameLineEdit->setText(fileName);
}

void AdminWidget::deleteEntry()
{

    QModelIndexList indexes = userTableView->selectionModel()->selectedIndexes();
    QModelIndex index;

    if (indexes.isEmpty())
        return;

    int ret = QMessageBox::information(this, tr("Deleting user(s)"),
                                       tr("Are you sure you want to delete user(s)?"),
                                       QMessageBox::Cancel, QMessageBox::Yes);

    if(ret == QMessageBox::Yes)
    {
        foreach(index, indexes)
        {
            QString role = userModel->record(index.row()).field("type").value().toString();
            if(role != "Administrator")
            {
                userModel->removeRow(index.row());
                mainWindowStatusBar->showMessage(tr("User(s) has been successfully deleted"));
            }
            else
            {
                QMessageBox::information(this, tr("Admin deletion"),
                                         tr("Sorry, you can not delete an admin user"));
                return;
            }
        }
    }
}

void AdminWidget::addEntry()
{
    UserDialog dlg(userModel, this);

    dlg.setWindowTitle(tr("Add User"));
    if(dlg.exec())
    {
        mainWindowStatusBar->showMessage(tr("User has been successfully added"));
    }
}

void AdminWidget::editEntry()
{
    QModelIndex index = userTableView->selectionModel()->currentIndex();
    QSqlRecord record = userModel->record(index.row());

    if(index.isValid())
    {
        UserDialog dlg(userModel, record, index.row(), this);
        dlg.setWindowTitle(tr("Edit User"));
        if(dlg.exec())
        {
            mainWindowStatusBar->showMessage(tr("User has been successfully updated"));
        }
    }
}
