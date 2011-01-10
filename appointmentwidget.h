#ifndef APPOINTMENTWIDGET_H
#define APPOINTMENTWIDGET_H

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
#include <QComboBox>
#include <QLineEdit>
#include "appointmentdialog.h"
#include "user.h"

class AppointmentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AppointmentWidget(QSqlRelationalTableModel* model,
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
    void setupTableView(QSqlRelationalTableModel *model);
    void setupModel(QSqlRelationalTableModel *model);

    QLabel *headerLabel;
    QTableView *appointmentTableView;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;

    QComboBox *filterComboBox;
    QLineEdit *filterLineEdit;

    QSqlRelationalTableModel *appointmentModel;
    QStatusBar *mainWindowStatusBar;

    QSortFilterProxyModel *appointmentSortModel;

    enum {Id, Start, End, Patient, Employee, Visit};

    User m_user;

};

#endif // APPOINTMENTWIDGET_H
