#include "employeewidget.h"

EmployeeWidget::EmployeeWidget(QSqlRelationalTableModel *model,
                               QStatusBar *statusBar, User user, QWidget *parent)
               :QWidget(parent), employeeModel(model), mainWindowStatusBar(statusBar), m_user(user)
{
    setupLayout();
    setupModel(model);
    setupTableView(model);

    employeeSortModel->setFilterKeyColumn(FullName);

    connect(addButton, SIGNAL(clicked()), this, SLOT(addEntry()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editEntry()));

    /* Permissions */
    if(m_user.role() == User::Admin)
    {
        connect(employeeTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editEntry()));
    }

    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteEntry()));
    connect(filterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterColumnChanged()));
    connect(filterLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onFilterLineEditChanged()));
}

void EmployeeWidget::updateTableView()
{
    setupModel(employeeModel);
}

void EmployeeWidget::setupLayout()
{
    headerLabel = new QLabel(tr("<h3>Employee Listing</h3>"));
    employeeTableView = new QTableView;

    addButton = new QPushButton(tr("&Add"));
    editButton = new QPushButton(tr("&Edit"));
    deleteButton = new QPushButton(tr("Delete"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    /* Permissions */
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
    filterComboBox->addItem(tr("Full Name"), FullName);
    filterComboBox->addItem(tr("Gender"), Gender);
    filterComboBox->addItem(tr("Occupation"), Occupation);
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
    mainLayout->addWidget(filterComboBox, 1, 2);
    mainLayout->addWidget(employeeTableView, 2, 0, 1, 0);
    mainLayout->addLayout(buttonLayout, 3, 2);

    setLayout(mainLayout);
}

void EmployeeWidget::setupModel(QSqlRelationalTableModel *model)
{
    model->setTable("employees");
    model->setRelation(Occupation, QSqlRelation("occupations", "id", "type"));
    model->setRelation(Country, QSqlRelation("countries", "id", "name"));
    model->setHeaderData(FirstName, Qt::Horizontal , tr("First Name"));
    model->setHeaderData(LastName, Qt::Horizontal , tr("Last Name"));
    model->setHeaderData(Gender, Qt::Horizontal , tr("Gender"));
    model->setHeaderData(Occupation, Qt::Horizontal , tr("Occupation"));
    model->setHeaderData(Country, Qt::Horizontal , tr("Country"));
    model->setHeaderData(CivilId, Qt::Horizontal , tr("Civil Id"));
    model->setHeaderData(FirstPhoneNumber, Qt::Horizontal , tr("Phone No. 1"));
    model->setHeaderData(SecondPhoneNumber, Qt::Horizontal , tr("Phone No. 2"));
    model->setHeaderData(Email, Qt::Horizontal , tr("Email"));
    model->select();
}

void EmployeeWidget::setupTableView(QSqlRelationalTableModel *model)
{
    employeeSortModel = new QSortFilterProxyModel;
    employeeSortModel->setSourceModel(model);
    employeeTableView->setModel(employeeSortModel);
    employeeTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    employeeTableView->setColumnHidden(Id, true);
    employeeTableView->setColumnHidden(FullName, true);
    employeeTableView->setColumnHidden(CreatedAt, true);
    employeeTableView->setColumnHidden(UpdatedAt, true);
    employeeTableView->horizontalHeader()->setStretchLastSection(true);
    employeeTableView->verticalHeader()->hide();
    employeeTableView->setSortingEnabled(true);
}

void EmployeeWidget::addEntry()
{
    EmployeeDialog dlg(employeeModel, this);

    dlg.setWindowTitle(tr("Add Employee"));
    if(dlg.exec())
    {
        mainWindowStatusBar->showMessage(tr("Employee has been successfully added"));
    }
}

void EmployeeWidget::editEntry()
{
    QModelIndex index = employeeTableView->selectionModel()->currentIndex();
    index = employeeSortModel->mapToSource(index);
    QSqlRecord record = employeeModel->record(index.row());

    if(index.isValid())
    {
        EmployeeDialog dlg(employeeModel, record, index.row(), this);
        dlg.setWindowTitle(tr("Edit Employee"));
        if(dlg.exec())
        {
            mainWindowStatusBar->showMessage(tr("Employee has been successfully updated"));
        }
    }
}

void EmployeeWidget::deleteEntry()
{
    QModelIndexList indexes = employeeTableView->selectionModel()->selectedIndexes();
    QModelIndex index;

    if (indexes.isEmpty())
        return;

    int ret = QMessageBox::information(this, tr("Deleting employee(s)"),
                                       tr("Are you sure you want to delete patient(s)?"),
                                       QMessageBox::Cancel, QMessageBox::Yes);

    if(ret == QMessageBox::Yes)
    {
        foreach(index, indexes)
        {
            index = employeeSortModel->mapToSource(index);
            employeeModel->removeRow(index.row());
            mainWindowStatusBar->showMessage(tr("Employee(s) has been successfully deleted"));
        }
    }
}


void EmployeeWidget::onFilterColumnChanged()
{
    employeeSortModel->setFilterKeyColumn(filterComboBox->itemData(filterComboBox->currentIndex()).toInt());
}

void EmployeeWidget::onFilterLineEditChanged()
{
    QRegExp regRxp(filterLineEdit->text(), Qt::CaseInsensitive);
    employeeSortModel->setFilterRegExp(regRxp);
}
