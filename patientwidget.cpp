#include "patientwidget.h"

PatientWidget::PatientWidget(QSqlRelationalTableModel *model,
                             QStatusBar *statusBar, User user, QWidget *parent)
              :QWidget(parent), patientModel(model), mainWindowStatusBar(statusBar), m_user(user)
{
    setupLayout();
    setupModel(model);
    setupTableView(model);

    patientSortModel->setFilterKeyColumn(FileNumber);

    connect(addButton, SIGNAL(clicked()), this, SLOT(addEntry()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editEntry()));

    /* Permissions */
    if(m_user.role() == User::Admin)
    {
        connect(patientTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editEntry()));
    }

    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteEntry()));
    connect(filterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterColumnChanged()));
    connect(filterLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onFilterLineEditChanged()));
}

void PatientWidget::updateTableView()
{
    setupModel(patientModel);
}

void PatientWidget::setupModel(QSqlRelationalTableModel *model)
{
    model->setTable("patients");
    model->setRelation(Country, QSqlRelation("countries", "id", "name"));
    model->setHeaderData(FileNumber, Qt::Horizontal, tr("File Number"));
    model->setHeaderData(FirstName, Qt::Horizontal, tr("First Name"));
    model->setHeaderData(LastName, Qt::Horizontal, tr("Last Name"));
    model->setHeaderData(Gender, Qt::Horizontal, tr("Gender"));
    model->setHeaderData(Country, Qt::Horizontal, tr("Country"));
    model->setHeaderData(CivilId, Qt::Horizontal, tr("Civil ID"));
    model->setHeaderData(FirstPhoneNumber, Qt::Horizontal, tr("Phone No. 1"));
    model->setHeaderData(SecondPhoneNumber, Qt::Horizontal, tr("Phone No. 2"));
    model->setHeaderData(Email, Qt::Horizontal, tr("Email"));
    model->select();
}

void PatientWidget::setupTableView(QSqlRelationalTableModel *model)
{
    patientSortModel = new QSortFilterProxyModel;
    patientSortModel->setSourceModel(model);
    patientTableView->setModel(patientSortModel);
    patientTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    patientTableView->setColumnHidden(Id, true);
    patientTableView->setColumnHidden(FullName, true);
    patientTableView->setColumnHidden(CreatedAt, true);
    patientTableView->setColumnHidden(UpdatedAt, true);
    patientTableView->horizontalHeader()->setStretchLastSection(true);
    patientTableView->verticalHeader()->hide();
    patientTableView->setSortingEnabled(true);
    patientTableView->resizeColumnsToContents();
}

void PatientWidget::setupLayout()
{
    headerLabel = new QLabel(tr("<h3>Patients Listing</h3>"));

    patientTableView = new QTableView;
    addButton = new QPushButton(tr("&Add"));
    editButton = new QPushButton(tr("&Edit"));
    deleteButton = new QPushButton(tr("Delete"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    if (m_user.role() != User::Viewer)
    {
        buttonLayout->addWidget(addButton);

        if(m_user.role() == User::Admin)
        {
            buttonLayout->addWidget(editButton);
            buttonLayout->addWidget(deleteButton);
        }
    }



    filterComboBox = new QComboBox;
    filterComboBox->addItem(tr("File Number"), FileNumber);
    filterComboBox->addItem(tr("Full Name"), FullName);
    filterComboBox->addItem(tr("Gender"), Gender);
    filterComboBox->addItem(tr("Country"), Country);
    filterComboBox->addItem(tr("Civil Id"), CivilId);
    filterComboBox->addItem(tr("First Phone Number"), FirstPhoneNumber);
    filterComboBox->addItem(tr("Second Phone Number"), SecondPhoneNumber);
    filterComboBox->addItem(tr("Email"), Email);

    filterLineEdit = new QLineEdit;

    QLabel *filterLabel = new QLabel(tr("&Filter:"));
    filterLabel->setBuddy(filterLineEdit);

    QHBoxLayout *filterLayout = new QHBoxLayout;
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(filterLineEdit);
    filterLayout->addWidget(filterComboBox);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(headerLabel, 0, 0);
    mainLayout->addLayout(filterLayout, 1, 0);
    mainLayout->addWidget(patientTableView, 2, 0, 1, 0);
    mainLayout->addLayout(buttonLayout, 3, 2);

    setLayout(mainLayout);
}

/* SLOTS */
void PatientWidget::deleteEntry()
{
    QModelIndexList indexes = patientTableView->selectionModel()->selectedIndexes();
    QModelIndex index;

    if (indexes.isEmpty())
        return;

    int ret = QMessageBox::information(this, tr("Deleting patient(s)"),
                                       tr("Are you sure you want to delete patient(s)?"),
                                       QMessageBox::Cancel, QMessageBox::Yes);

    if(ret == QMessageBox::Yes)
    {
        foreach(index, indexes)
        {
            index = patientSortModel->mapToSource(index);
            patientModel->removeRow(index.row());
            mainWindowStatusBar->showMessage(tr("Patient(s) has been successfully deleted"));
        }
    }
}

void PatientWidget::addEntry()
{
    PatientDialog dlg(patientModel, this);

    dlg.setWindowTitle(tr("Add Patient"));
    if(dlg.exec())
    {
        mainWindowStatusBar->showMessage(tr("Patient has been successfully added"));
    }
}

void PatientWidget::editEntry()
{
    QModelIndex index = patientTableView->selectionModel()->currentIndex();
    index = patientSortModel->mapToSource(index);
    QSqlRecord record = patientModel->record(index.row());

    if(index.isValid())
    {
        PatientDialog dlg(patientModel, record, index.row(), this);
        dlg.setWindowTitle(tr("Edit Patient"));
        if(dlg.exec())
        {
            mainWindowStatusBar->showMessage(tr("Patient has been successfully updated"));
        }
    }
}

void PatientWidget::onFilterColumnChanged()
{
    patientSortModel->setFilterKeyColumn(filterComboBox->itemData(filterComboBox->currentIndex()).toInt());
}

void PatientWidget::onFilterLineEditChanged()
{
    QRegExp regRxp(filterLineEdit->text(), Qt::CaseInsensitive);
    patientSortModel->setFilterRegExp(regRxp);
}
