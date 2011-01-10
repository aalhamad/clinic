#ifndef EMPLOYEEWIDGET_H
#define EMPLOYEEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTableView>
#include <QPushButton>
#include <QSqlRelationalTableModel>
#include <QStatusBar>
#include <QSortFilterProxyModel>
#include <QGridLayout>
#include <QSqlRelationalDelegate>
#include <QSqlRecord>
#include <QHeaderView>
#include <QMessageBox>
#include "employeedialog.h"
#include "user.h"

class EmployeeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EmployeeWidget(QSqlRelationalTableModel* model,
                            QStatusBar* statusBar,
                            User user,
                            QWidget *parent = 0);
    void updateTableView();

private slots:
    void addEntry();
    void editEntry();
    void deleteEntry();
    void onFilterColumnChanged();
    void onFilterLineEditChanged();

private:
    void setupLayout();
    void setupModel(QSqlRelationalTableModel *model);
    void setupTableView(QSqlRelationalTableModel *model);

    QLabel *headerLabel;
    QTableView *employeeTableView;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;

    QComboBox *filterComboBox;
    QLineEdit *filterLineEdit;

    QSqlRelationalTableModel *employeeModel;
    QStatusBar *mainWindowStatusBar;

    QSortFilterProxyModel *employeeSortModel;

    enum { Id, FirstName, LastName, FullName,
           Gender, Occupation, Country, CivilId,
           FirstPhoneNumber, SecondPhoneNumber,
           Email, CreatedAt, UpdatedAt };

    User m_user;
};

#endif // EMPLOYEEWIDGET_H
