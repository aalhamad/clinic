#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableView>
#include <QSettings>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QStatusBar>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QHeaderView>
#include "globals.h"
#include "userdialog.h"

class AdminWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdminWidget(QSqlDatabase& database,
                         QStatusBar* statusBar, QWidget *parent = 0);

    void updateTableView();

private slots:
    void onChangeFileNameToolButtonClicked();
    void onApplyButtonClicked();
    void onRevertButtonClicked();
    void editEntry();
    void addEntry();
    void deleteEntry();

private:
    void setupLayout();
    void setupModels();
    void setupUserTableView();

    QLineEdit *fileNameLineEdit;
    QToolButton *changeFileNameToolButton;
    QTableView *userTableView;
    QPushButton *applyButton;
    QPushButton *revertButton;

    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;

    QStatusBar *mainWindowStatusBar;
    QSqlDatabase m_db;
    QSqlRelationalTableModel *userModel;

    enum { Id, Username, Email, HashedPassword, Salt ,Role };
};

#endif // ADMINWIDGET_H
