#ifndef ANALYSISWIDGET_H
#define ANALYSISWIDGET_H

#include <QWidget>
#include <QTextBrowser>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTimeEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSqlError>

class AnalysisWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnalysisWidget(QSqlDatabase &database, QWidget *parent = 0);

private slots:
    void onAnalysisbuttonClicked();

private:
    void setupLayout();

    QDateTimeEdit *fromDateTimeEdit;
    QDateTimeEdit *toDateTimeEdit;
    QPushButton *analysisButton;
    QTextBrowser *displayTextBrowser;
    QSqlDatabase db;
};

#endif // ANALYSISWIDGET_H
