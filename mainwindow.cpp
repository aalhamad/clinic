#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    readSettings();
    setupDatabase();

    if(!login())
    {
        exit(-1);
    }

    setupLayout();
    createStatusBar();
    createActions();
    createMenus();

    connect(patientButton, SIGNAL(clicked()), this, SLOT(showPatientWidget()));
    connect(appointmentButton, SIGNAL(clicked()), this, SLOT(showAppointmentWidget()));
    connect(employeeButton, SIGNAL(clicked()), this, SLOT(showEmployeeWidget()));
    connect(invoiceButton, SIGNAL(clicked()), this, SLOT(showInvoiceWidget()));
    connect(analysisButton, SIGNAL(clicked()), this, SLOT(showAnalysisWidget()));
    connect(adminButton, SIGNAL(clicked()), this, SLOT(showAdminWidget()));


    setWindowTitle(tr("%1").arg(QCoreApplication::applicationName()));
}

MainWindow::~MainWindow()
{
    m_db.close();
    m_db.removeDatabase(m_db.connectionName());
}

/* Private Functions */
bool MainWindow::login()
{
    LoginDialog dlg;
    bool result = false;

    dlg.setWindowTitle(tr("Login"));

    if(dlg.exec())
    {
        bool sucess = User::authenticate(dlg.username(), dlg.password());
        if(sucess)
        {
            m_user = User::find(dlg.username());
            result = true;
        }
    }
    return result;
}

void MainWindow::writeSettings()
{
    QSettings settings(Globals::OrganizationName, Globals::ApplicationName);
    settings.setValue("size", size());
}

void MainWindow::readSettings()
{
    QSettings settings(Globals::OrganizationName, Globals::ApplicationName);
    QSize size = settings.value("size", QSize(WINDOW_DEFAULT_WIDTH, sizeHint().height())).toSize();
    QString fileName = settings.value(Globals::FileDbDestination, "").toString();
    m_dbFileName = fileName;
    resize(size);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::information(this, tr("Quit"),
                                       tr("Are you sure you want to quit?"),
                                       QMessageBox::Cancel, QMessageBox::Yes);

    if(ret == QMessageBox::Yes)
    {
        event->accept();
        writeSettings();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::setupDatabase()
{
    QSettings settings(Globals::OrganizationName, Globals::ApplicationName);
    m_dbFileName = settings.value(Globals::FileDbDestination, "").toString();

    bool fileExists = QFile::exists(m_dbFileName);
    if(!fileExists)
    {
        createDatabaseWizard();
    }
    else
    {
        createDatabaseConnection();
    }

}

void MainWindow::createDatabaseConnection()
{

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_dbFileName);

    if(!m_db.open())
    {
        QMessageBox::warning(this, tr("Cannot open database"),
                             tr("Unable to establish a database connection.\n"
                                 "Contact system administration for support.\n"
                                 "Error: %1").arg(m_db.lastError().text()),
                              QMessageBox::Ok);
        createDatabaseWizard();
    }
}

void MainWindow::createDatabase()
{
    QProgressDialog progress;
    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowTitle(QCoreApplication::applicationName());
    progress.setLabelText(tr("Creating database..."));
    progress.setWindowTitle(tr("%1").arg(QCoreApplication::applicationName()));
    progress.setMaximum(250);
    progress.setValue(1);
    qApp->processEvents();
    QFile file(":/db/sqlite.sql");
    QSqlQuery query;
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::critical(this, tr("File Error"),
                              tr("Unable to open countries file.\n Error: %1").arg(file.errorString()));
    }
    else
    {
        int count = 0;
        QString text;
        while(!file.atEnd())
        {
            text = file.readLine().trimmed();
            if(!text.isEmpty())
            {
                query.exec(text);
                count++;
                progress.setValue(count);
            }
        }
    }

    progress.setValue(progress.maximum());
    qApp->processEvents();
}

void MainWindow::createDatabaseWizard()
{
    QWizard wizard;
    wizard.setWindowTitle(tr("Wizard - %1").arg(QCoreApplication::applicationName()));
    wizard.addPage(new IntroDbWizardPage());
    wizard.addPage(new CreateDbWizardPage());
    wizard.addPage(new AdminDbWizardPage());
    wizard.setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo.png"));

    if(wizard.exec())
    {

        QString fileName = wizard.field("fileName").toString();
        bool createDb = wizard.field("cbCreateDb").toBool();
        bool fileExists = QFile::exists(fileName);

        QString password = wizard.field("password").toString();
        QString username = wizard.field("username").toString();
        QString email = wizard.field("email").toString();

        if(fileExists && createDb)
        {
            m_dbFileName = fileName;
            QSettings settings(Globals::OrganizationName, Globals::ApplicationName);
            settings.setValue(Globals::FileDbDestination, m_dbFileName);
            createDatabaseConnection();
        }
        else if(!fileExists && !createDb)
        {
            m_dbFileName = fileName;
            QSettings settings(Globals::OrganizationName, Globals::ApplicationName);
            settings.setValue(Globals::FileDbDestination, m_dbFileName);
            createDatabaseConnection();
            createDatabase();
            QSqlQuery query;
            query.prepare("INSERT INTO users (username, email, hashed_password, salt, role_id) "
                          "VALUES (:username, :email, :hashed_password, :salt, :role_id)");
            query.bindValue(":username", username);
            query.bindValue(":email", email);
            QString salt = QUuid::createUuid().toString();
            QString hashed_password = QCryptographicHash::hash(password.toAscii() + salt.toAscii(),
                                                               QCryptographicHash::Sha1);
            query.bindValue(":hashed_password", hashed_password);
            query.bindValue(":salt", salt);
            query.bindValue(":role_id", 1);
            query.exec();
        }
    }
    else
    {
        QMessageBox::information(this, tr("Fail Database Creation"),
                                 tr("Failed to create database."));
        exit(-1);
    }
}

void MainWindow::setupLayout()
{
    centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    patientModel = new QSqlRelationalTableModel(this, m_db);
    appointmentModel = new Appointmentrelationtablemodel(this, m_db);
    employeeModel = new QSqlRelationalTableModel(this, m_db);
    invoiceModel = new InvoiceRelationTableModel(this, m_db);

    patientWidget = new PatientWidget(patientModel, statusBar(), m_user);
    appointmentWidget = new AppointmentWidget(appointmentModel, statusBar(), m_user);
    employeeWidget = new EmployeeWidget(employeeModel, statusBar(), m_user);
    invoiceWidget = new InvoiceWidget(invoiceModel, statusBar(), m_user);
    analysisWidget = new AnalysisWidget(m_db);
    adminWidget = new AdminWidget(m_db, statusBar());

    stackWidget = new QStackedWidget;
    stackWidget->addWidget(patientWidget);
    stackWidget->addWidget(appointmentWidget);
    stackWidget->addWidget(employeeWidget);
    stackWidget->addWidget(invoiceWidget);
    stackWidget->addWidget(analysisWidget);
    stackWidget->addWidget(adminWidget);

    QSize buttonDefaultSize(SIDE_BUTTON_SIZE, SIDE_BUTTON_SIZE);
    patientButton = new QPushButton;
    patientButton->setMinimumSize(buttonDefaultSize);
    patientButton->setCheckable(true);
    patientButton->setIcon(QIcon(":/images/patients.png"));
    patientButton->setIconSize(buttonDefaultSize);
    patientButton->setToolTip(tr("Patients"));
    patientButton->setStatusTip(tr("Create, edit or delete patients"));

    appointmentButton = new QPushButton;
    appointmentButton->setMinimumSize(buttonDefaultSize);
    appointmentButton->setCheckable(true);
    appointmentButton->setIcon(QIcon(":/images/appointments.png"));
    appointmentButton->setIconSize(buttonDefaultSize);
    appointmentButton->setToolTip(tr("Appointments"));
    appointmentButton->setStatusTip(tr("Create, edit or delete appointments"));

    employeeButton = new QPushButton;
    employeeButton->setMinimumSize(buttonDefaultSize);
    employeeButton->setCheckable(true);
    employeeButton->setIcon(QIcon(":/images/employees.png"));
    employeeButton->setIconSize(buttonDefaultSize);
    employeeButton->setToolTip(tr("Employees"));
    employeeButton->setStatusTip(tr("Create, edit or delete employees"));

    invoiceButton = new QPushButton;
    invoiceButton->setMinimumSize(buttonDefaultSize);
    invoiceButton->setCheckable(true);
    invoiceButton->setIcon(QIcon(":/images/invoices.png"));
    invoiceButton->setIconSize(buttonDefaultSize);
    invoiceButton->setToolTip(tr("Invoices"));
    invoiceButton->setStatusTip(tr("Create, edit or delete invoices"));

    analysisButton = new QPushButton;
    analysisButton->setMinimumSize(buttonDefaultSize);
    analysisButton->setCheckable(true);
    analysisButton->setIcon(QIcon(":/images/analysis.png"));
    analysisButton->setIconSize(buttonDefaultSize);
    analysisButton->setToolTip(tr("Analysis"));
    analysisButton->setStatusTip(tr("Show income statment, analysis report, etc."));

    adminButton = new QPushButton;
    adminButton->setMinimumSize(buttonDefaultSize);
    adminButton->setCheckable(true);
    adminButton->setIcon(QIcon(":/images/admin.png"));
    adminButton->setIconSize(buttonDefaultSize);
    adminButton->setToolTip(tr("Admin"));
    adminButton->setStatusTip("Application and users settings");

    QVBoxLayout *buttonsLayout = new QVBoxLayout;
    buttonsLayout->addWidget(patientButton);
    buttonsLayout->addWidget(appointmentButton);
    buttonsLayout->addWidget(employeeButton);
    buttonsLayout->addWidget(invoiceButton);
    buttonsLayout->addWidget(analysisButton);
    buttonsLayout->addWidget(adminButton);
    buttonsLayout->addStretch();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(buttonsLayout, 0, 0);
    mainLayout->addWidget(stackWidget, 0, 1);

    /* Permissions */
    if(m_user.role() != User::Admin)
    {
        analysisButton->hide();
        analysisWidget->hide();
        adminButton->hide();
        adminWidget->hide();
    }

    if(m_user.role() == User::Viewer)
    {
        invoiceButton->hide();
        invoiceWidget->hide();
    }

    centralWidget->setLayout(mainLayout);

}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage("");
}

void MainWindow::createActions()
{
    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    userSettingsAction = new QAction(tr("User Settings"), this);
    connect(userSettingsAction, SIGNAL(triggered()), this, SLOT(showUserSettings()));
    userSettingsAction->setStatusTip(tr("Show or Edit user settings"));

    aboutAction = new QAction(tr("About"), this);
    aboutAction->setStatusTip(tr("Show application about"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(userSettingsAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::showUserSettings()
{
    m_user = User::find(m_user.username());
    UserSettingsDialog dlg(m_user, this);
    dlg.setWindowTitle(tr("User Settings"));
    if(dlg.exec())
    {
        statusBar()->showMessage(tr("User settings have been successfully updated."));
    }
}

void MainWindow::about()
{
    AboutDialog dlg;
    dlg.setWindowTitle(tr("About"));
    dlg.exec();
}

void MainWindow::showAnalysisWidget()
{
    stackWidget->setCurrentWidget(analysisWidget);
    appointmentButton->setChecked(false);
    employeeButton->setChecked(false);
    invoiceButton->setChecked(false);
    patientButton->setChecked(false);
    adminButton->setChecked(false);
}

void MainWindow::showPatientWidget()
{
    stackWidget->setCurrentWidget(patientWidget);
    patientWidget->updateTableView();
    appointmentButton->setChecked(false);
    employeeButton->setChecked(false);
    invoiceButton->setChecked(false);
    analysisButton->setChecked(false);
    adminButton->setChecked(false);
}

void MainWindow::showAppointmentWidget()
{
    stackWidget->setCurrentWidget(appointmentWidget);
    appointmentWidget->updateTableView();
    patientButton->setChecked(false);
    employeeButton->setChecked(false);
    invoiceButton->setChecked(false);
    analysisButton->setChecked(false);
    adminButton->setChecked(false);
}

void MainWindow::showEmployeeWidget()
{
    stackWidget->setCurrentWidget(employeeWidget);
    employeeWidget->updateTableView();
    patientButton->setChecked(false);
    appointmentButton->setChecked(false);
    invoiceButton->setChecked(false);
    analysisButton->setChecked(false);
    adminButton->setChecked(false);
}

void MainWindow::showInvoiceWidget()
{
    stackWidget->setCurrentWidget(invoiceWidget);
    invoiceWidget->updateTableView();
    employeeButton->setChecked(false);
    patientButton->setChecked(false);
    appointmentButton->setChecked(false);
    analysisButton->setChecked(false);
    adminButton->setChecked(false);
}

void MainWindow::showAdminWidget()
{
    stackWidget->setCurrentWidget(adminWidget);
    invoiceButton->setChecked(false);
    employeeButton->setChecked(false);
    patientButton->setChecked(false);
    appointmentButton->setChecked(false);
    analysisButton->setChecked(false);
}

