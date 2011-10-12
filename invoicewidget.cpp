#include "invoicewidget.h"

InvoiceWidget::InvoiceWidget(QSqlRelationalTableModel *model,
                             QStatusBar *statusBar, User user, QWidget *parent)
              :QWidget(parent), invoiceModel(model), mainWindowStatusBar(statusBar), m_user(user)
{

    setupLayout();
    setupModel(model);
    setupTableView(model);

    invoiceSortModel->setFilterKeyColumn(Patient);

    connect(addButton, SIGNAL(clicked()), this, SLOT(addEntry()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editEntry()));

    /* Permissions */
    if(m_user.role() == User::Admin)
    {
        connect(invoiceTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editEntry()));
    }

    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteEntry()));
    connect(printButton, SIGNAL(clicked()), this, SLOT(print()));
    connect(filterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterColumnChanged()));
    connect(filterLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onFilterLineEditChanged()));
}

void InvoiceWidget::updateTableView()
{
    setupModel(invoiceModel);
}

void InvoiceWidget::setupLayout()
{
    headerLabel = new QLabel(tr("<h3>Invoice Listing</h3>"));

    invoiceTableView = new QTableView;
    addButton = new QPushButton(tr("&Add"));
    editButton = new QPushButton(tr("&Edit"));
    deleteButton = new QPushButton(tr("Delete"));

    QSize default_size(DEFAULT_PRINT_HEIGH, DEFAULT_PRINT_WIDTH);
    printButton = new QPushButton;
    printButton->setIcon(QIcon(":/images/print.png"));
    printButton->setIconSize(default_size);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();

    /* Permissions */
    buttonLayout->addWidget(addButton);
    if(m_user.role() == User::Admin)
    {
        buttonLayout->addWidget(editButton);
        buttonLayout->addWidget(deleteButton);
    }

    filterComboBox = new QComboBox;
    filterComboBox->addItem(tr("Patient"), Patient);
    filterComboBox->addItem(tr("Vist Type"), Visit);

    filterLineEdit = new QLineEdit;

    QLabel *filterLabel = new QLabel(tr("&Filter:"));
    filterLabel->setBuddy(filterLineEdit);

    QHBoxLayout *filterLayout = new QHBoxLayout;
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(filterLineEdit);
    filterLayout->addWidget(filterComboBox);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(headerLabel, 0, 0);
    mainLayout->addWidget(printButton, 0, 2, Qt::AlignRight);
    mainLayout->addLayout(filterLayout, 1, 0);
    mainLayout->addWidget(filterLineEdit, 1, 1);
    mainLayout->addWidget(filterComboBox, 1, 2);
    mainLayout->addWidget(invoiceTableView, 2, 0, 1, 0);
    mainLayout->addLayout(buttonLayout, 3, 2);

    setLayout(mainLayout);
}

void InvoiceWidget::setupModel(QSqlRelationalTableModel *model)
{
    model->setTable("invoices");
    model->setRelation(Patient, QSqlRelation("patients", "id", "full_name"));
    model->setRelation(Visit, QSqlRelation("visits", "id", "type"));
    model->setHeaderData(Patient, Qt::Horizontal, tr("Patient"));
    model->setHeaderData(Date, Qt::Horizontal, tr("Date and Time"));
    model->setHeaderData(Price, Qt::Horizontal, tr("Price"));
    model->setHeaderData(Visit, Qt::Horizontal, tr("Visit"));
    model->select();
}

void InvoiceWidget::setupTableView(QSqlRelationalTableModel *model)
{
    invoiceSortModel = new QSortFilterProxyModel;
    invoiceSortModel->setSourceModel(model);
    invoiceTableView->setModel(invoiceSortModel);
    invoiceTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    invoiceTableView->setColumnHidden(Id, true);
    invoiceTableView->horizontalHeader()->setStretchLastSection(true);
    invoiceTableView->verticalHeader()->hide();
    invoiceTableView->setSortingEnabled(true);
    invoiceTableView->resizeColumnsToContents();

}


void InvoiceWidget::addEntry()
{
    InvoiceDialog dlg(invoiceModel, this);
    dlg.setWindowTitle(tr("Add Invoice"));
    if(dlg.exec())
    {
        mainWindowStatusBar->showMessage(tr("Invoice has been successfully added"));
    }
}

void InvoiceWidget::editEntry()
{
    QModelIndex index = invoiceTableView->selectionModel()->currentIndex();
    index = invoiceSortModel->mapToSource(index);
    QSqlRecord record = invoiceModel->record(index.row());

    if(index.isValid())
    {
        InvoiceDialog dlg(invoiceModel, record, index.row(), this);
        dlg.setWindowTitle(tr("Edit Invoice"));
        if(dlg.exec())
        {
            mainWindowStatusBar->showMessage(tr("Invoice has been successfully updated"));
        }
    }
}

void InvoiceWidget::deleteEntry()
{
    QModelIndexList indexes = invoiceTableView->selectionModel()->selectedIndexes();
    QModelIndex index;

    if (indexes.isEmpty())
        return;

    int ret = QMessageBox::information(this, tr("Deleting invoice(s)"),
                                       tr("Are you sure you want to delete invoice(s)?"),
                                       QMessageBox::Cancel, QMessageBox::Yes);

    if(ret == QMessageBox::Yes)
    {
        foreach(index, indexes)
        {
            index = invoiceSortModel->mapToSource(index);
            invoiceModel->removeRow(index.row());
            mainWindowStatusBar->showMessage(tr("Invoice(s) has been successfully deleted"));
        }
    }
}

void InvoiceWidget::print()
{
    QModelIndex index = invoiceTableView->selectionModel()->currentIndex();
    if(!index.isValid())
        return;

    QPrinter printer;
    QPrintPreviewDialog printDlg(&printer);
    connect(&printDlg, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printPreview(QPrinter *)));
    printDlg.exec();
}

void InvoiceWidget::printPreview(QPrinter *printer)
{

    QModelIndex index = invoiceTableView->selectionModel()->currentIndex();
    QSqlRecord record = invoiceModel->record(index.row());

    QString fullname = record.field("full_name").value().toString();
    QDateTime datetime = record.field("date").value().toDateTime();
    QString price = record.field("price").value().toString();
    QString visitType = record.field("type").value().toString();

    Invoice invoice(fullname, datetime, price, visitType);

    textDoc = new QTextDocument;
    createInvoice(textDoc, invoice);
    textDoc->print(printer);
}


void InvoiceWidget::onFilterColumnChanged()
{
    invoiceSortModel->setFilterKeyColumn(filterComboBox->itemData(filterComboBox->currentIndex()).toInt());
}

void InvoiceWidget::onFilterLineEditChanged()
{
    QRegExp regRxp(filterLineEdit->text(), Qt::CaseInsensitive);
    invoiceSortModel->setFilterRegExp(regRxp);
}

void InvoiceWidget::createInvoice(QTextDocument *doc, const Invoice &invoice)
{
    QString clinic_image(":/invoice_template/clinic-name-ar.png");
    QString logo(":/invoice_template/logo.png");
    QString no_refunds(":/invoice_template/no-refunds.png");
    QString good_health(":/invoice_template/good-health.png");
    QString address_en(":/invoice_template/address-en.png");
}
