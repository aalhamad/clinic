#ifndef PATIENTWIDGET_H
#define PATIENTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTableView>
#include <QPushButton>
#include <QSqlRelationalTableModel>
#include <QStatusBar>
#include <QGridLayout>
#include <QSqlRelationalDelegate>
#include <QSqlRecord>
#include <QHeaderView>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include "patientdialog.h"
#include "user.h"

class PatientWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PatientWidget(QSqlRelationalTableModel* model,
                           QStatusBar* statusBar,
                           User user,
                           QWidget *parent = 0);

    enum { Id, FileNumber, FirstName, LastName, FullName,
           Gender, Country, CivilId, FirstPhoneNumber,
           SecondPhoneNumber, Email, CreatedAt, UpdatedAt };

    void updateTableView();

private slots:
    void addEntry();
    void editEntry();
    void deleteEntry();
    void onFilterLineEditChanged();
    void onFilterColumnChanged();

private:
    void setupLayout();
    void setupModel(QSqlRelationalTableModel *model);
    void setupTableView(QSqlRelationalTableModel *model);

    QLabel *headerLabel;
    QTableView *patientTableView;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;

    QComboBox *filterComboBox;
    QLineEdit *filterLineEdit;

    QSqlRelationalTableModel *patientModel;
    QSortFilterProxyModel *patientSortModel;
    QStatusBar *mainWindowStatusBar;

    User m_user;
};

#endif // PATIENTWIDGET_H
