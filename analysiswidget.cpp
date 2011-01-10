#include "analysiswidget.h"

AnalysisWidget::AnalysisWidget( QSqlDatabase &database, QWidget *parent)
               : QWidget(parent), db(database)
{
    setupLayout();
    connect(analysisButton, SIGNAL(clicked()), this, SLOT(onAnalysisbuttonClicked()));
}

void AnalysisWidget::setupLayout()
{
    displayTextBrowser = new QTextBrowser;

    fromDateTimeEdit = new QDateTimeEdit;
    toDateTimeEdit = new QDateTimeEdit;
    fromDateTimeEdit->setCalendarPopup(true);
    toDateTimeEdit->setCalendarPopup(true);
    fromDateTimeEdit->setDate(QDate::currentDate());
    toDateTimeEdit->setDate(QDate::currentDate().addDays(1));

    QLabel *fromLabel = new QLabel(tr("&From:"));
    QLabel *toLabel = new QLabel(tr("&To:"));
    fromLabel->setBuddy(fromDateTimeEdit);
    toLabel->setBuddy(toDateTimeEdit);

    analysisButton = new QPushButton(tr("Analysis"));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(fromLabel);
    topLayout->addWidget(fromDateTimeEdit);
    topLayout->addWidget(toLabel);
    topLayout->addWidget(toDateTimeEdit);
    topLayout->addStretch();
    topLayout->addWidget(analysisButton);


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(displayTextBrowser);
    setLayout(mainLayout);
}


void AnalysisWidget::onAnalysisbuttonClicked()
{
    QSqlQuery query(db);
    query.prepare("SELECT SUM(price) as total_sum FROM invoices WHERE date >= :from AND date <= :to");
    query.bindValue(":from", fromDateTimeEdit->dateTime());
    query.bindValue(":to", toDateTimeEdit->dateTime());
    query.exec();
    query.next();
    QString html = "Total Income: " + query.value(0).toString();
    displayTextBrowser->setText(html);
}
