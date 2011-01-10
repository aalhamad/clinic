#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSettings>
#include <QCloseEvent>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QSqlError>
#include <QMessageBox>
#include <QStatusBar>
#include <QFile>
#include <QProgressDialog>
#include <QSqlQuery>
#include <QDir>
#include <QCoreApplication>
#include <QWizard>
#include <QDebug>
#include <QCryptographicHash>
#include <QUuid>
#include <QMenuBar>
#include <QAction>
#include "patientwidget.h"
#include "appointmentwidget.h"
#include "employeewidget.h"
#include "invoicewidget.h"
#include "analysiswidget.h"
#include "invoicerelationtablemodel.h"
#include "appointmentrelationtablemodel.h"
#include "introdbwizardpage.h"
#include "createdbwizardpage.h"
#include "admindbwizardpage.h"
#include "logindialog.h"
#include "user.h"
#include "adminwidget.h"
#include "usersettingsdialog.h"
#include "globals.h"
#include "aboutdialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);

private slots:
    void showPatientWidget();
    void showAppointmentWidget();
    void showEmployeeWidget();
    void showInvoiceWidget();
    void showAnalysisWidget();
    void showAdminWidget();

    void showUserSettings();
    void about();

private:
    void setupLayout();
    void setupDatabase();
    void createStatusBar();
    void createDatabase();
    void createActions();
    void createMenus();
    void readSettings();
    void writeSettings();
    void createDatabaseWizard();
    void createDatabaseConnection();
    bool login();

    enum { SIDE_BUTTON_SIZE = 75, WINDOW_DEFAULT_WIDTH = 800 };

    QPushButton *patientButton;
    QPushButton *appointmentButton;
    QPushButton *employeeButton;
    QPushButton *invoiceButton;
    QPushButton *analysisButton;
    QPushButton *adminButton;

    QMenu *fileMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;

    QAction *exitAction;
    QAction *aboutAction;
    QAction *userSettingsAction;

    QWidget *centralWidget;
    QStackedWidget *stackWidget;
    PatientWidget *patientWidget;
    AppointmentWidget *appointmentWidget;
    EmployeeWidget *employeeWidget;
    InvoiceWidget *invoiceWidget;
    AnalysisWidget *analysisWidget;
    AdminWidget *adminWidget;

    QSqlRelationalTableModel *patientModel;
    QSqlRelationalTableModel *employeeModel;
    Appointmentrelationtablemodel *appointmentModel;
    InvoiceRelationTableModel *invoiceModel;

    QString m_dbFileName;
    QSqlDatabase m_db;
    User m_user;

};

#endif // MAINWINDOW_H
