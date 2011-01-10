#include "appointmentwidget.h"

AppointmentWidget::AppointmentWidget(QSqlRelationalTableModel *model,
                                     QStatusBar *statusBar, User user ,QWidget *parent)
                  :QWidget(parent), appointmentModel(model), mainWindowStatusBar(statusBar), m_user(user)
{
    setupLayout();
    setupModel(model);
    setupTableView(model);

    appointmentSortModel->setFilterKeyColumn(Patient);

    connect(addButton, SIGNAL(clicked()), this, SLOT(addEntry()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editEntry()));

    /* Permissions */
    if(m_user.role() == User::Admin)
    {
        connect(appointmentTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editEntry()));
    }

    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteEntry()));
    connect(filterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterColumnChanged()));
    connect(filterLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onFilterLineEditChanged()));
}

void AppointmentWidget::updateTableView()
{
    setupModel(appointmentModel);
}

/* Private Functions */
void AppointmentWidget::setupLayout()
{
    headerLabel = new QLabel(tr("<h3>Appointments Listing</h3>"));
    appointmentTableView = new QTableView;

    addButton = new QPushButton(tr("&Add"));
    editButton = new QPushButton(tr("&Edit"));
    deleteButton = new QPushButton(tr("Delete"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();

    /* Permission */
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
    filterComboBox->addItem(tr("Patient"), Patient);
    filterComboBox->addItem(tr("Employee"), Employee);
    filterComboBox->addItem(tr("Visit Type"), Visit);

    filterLineEdit = new QLineEdit;
    QLabel *filterLabel = new QLabel(tr("&Filter: "));
    filterLabel->setBuddy(filterLineEdit);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(headerLabel, 0, 0);
    mainLayout->addWidget(filterLabel, 1, 0);
    mainLayout->addWidget(filterLineEdit, 1, 1);
    mainLayout->addWidget(filterComboBox, 1, 2);
    mainLayout->addWidget(appointmentTableView, 2, 0, 1, 0);
    mainLayout->addLayout(buttonLayout, 3, 2);

    setLayout(mainLayout);
}

void AppointmentWidget::setupModel(QSqlRelationalTableModel *model)
{
    model->setTable("appointments");
    model->setRelation(Patient, QSqlRelation("patients", "id", "full_name"));
    model->setRelation(Employee, QSqlRelation("employees", "id", "full_name"));
    model->setRelation(Visit, QSqlRelation("visits", "id", "type"));
    model->setHeaderData(Start, Qt::Horizontal, tr("Start"));
    model->setHeaderData(End, Qt::Horizontal, tr("End"));
    model->setHeaderData(Patient, Qt::Horizontal, tr("Patient"));
    model->setHeaderData(Employee, Qt::Horizontal, tr("Employee"));
    model->setHeaderData(Visit, Qt::Horizontal, tr("Visit"));
    model->select();
}

void AppointmentWidget::setupTableView(QSqlRelationalTableModel *model)
{
    appointmentSortModel = new QSortFilterProxyModel;
    appointmentSortModel->setSourceModel(model);
    appointmentTableView->setModel(appointmentSortModel);
    appointmentTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    appointmentTableView->setColumnHidden(Id, true);
    appointmentTableView->horizontalHeader()->setStretchLastSection(true);
    appointmentTableView->verticalHeader()->hide();
    appointmentTableView->setSortingEnabled(true);
    appointmentTableView->resizeColumnsToContents();
}

void AppointmentWidget::addEntry()
{
    AppointmentDialog dlg(appointmentModel, this);

    dlg.setWindowTitle(tr("Add Appointment"));
    if(dlg.exec())
    {
        mainWindowStatusBar->showMessage(tr("Appointment has been successfully added"));
    }
}

void AppointmentWidget::editEntry()
{
    QModelIndex index = appointmentTableView->selectionModel()->currentIndex();
    index = appointmentSortModel->mapToSource(index);
    QSqlRecord record = appointmentModel->record(index.row());

    if(index.isValid())
    {
        AppointmentDialog dlg(appointmentModel, record, index.row(), this);
        dlg.setWindowTitle(tr("Edit Appointment"));
        if(dlg.exec())
        {
            mainWindowStatusBar->showMessage(tr("Appointment has been successfully updated"));
        }
    }
}

void AppointmentWidget::deleteEntry()
{
    QModelIndexList indexes = appointmentTableView->selectionModel()->selectedIndexes();
    QModelIndex index;

    if (indexes.isEmpty())
        return;

    int ret = QMessageBox::information(this, tr("Deleting appointment(s)"),
                                       tr("Are you sure you want to delete appointment(s)?"),
                                       QMessageBox::Cancel, QMessageBox::Yes);

    if(ret == QMessageBox::Yes)
    {
        foreach(index, indexes)
        {
            index = appointmentSortModel->mapToSource(index);
            appointmentModel->removeRow(index.row());
            mainWindowStatusBar->showMessage(tr("Appointment(s) has been successfully deleted"));
        }
    }
}

void AppointmentWidget::onFilterColumnChanged()
{
    appointmentSortModel->setFilterKeyColumn(filterComboBox->itemData(filterComboBox->currentIndex()).toInt());
}

void AppointmentWidget::onFilterLineEditChanged()
{
    QRegExp regRxp(filterLineEdit->text(), Qt::CaseInsensitive);
    appointmentSortModel->setFilterRegExp(regRxp);
}
