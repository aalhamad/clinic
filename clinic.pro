#-------------------------------------------------
#
# Project created by QtCreator 2011-01-09T13:10:43
#
#-------------------------------------------------

QT       += core gui sql

TARGET = clinic
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    usersettingsdialog.cpp \
    userdialog.cpp \
    user.cpp \
    patientwidget.cpp \
    patientdialog.cpp \
    logindialog.cpp \
    invoicewidget.cpp \
    invoicerelationtablemodel.cpp \
    invoicedialog.cpp \
    introdbwizardpage.cpp \
    employeewidget.cpp \
    employeedialog.cpp \
    createdbwizardpage.cpp \
    appointmentwidget.cpp \
    appointmentrelationtablemodel.cpp \
    appointmentdialog.cpp \
    analysiswidget.cpp \
    adminwidget.cpp \
    admindbwizardpage.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    usersettingsdialog.h \
    userdialog.h \
    user.h \
    patientwidget.h \
    patientdialog.h \
    logindialog.h \
    invoicewidget.h \
    invoicerelationtablemodel.h \
    invoicedialog.h \
    introdbwizardpage.h \
    globals.h \
    employeewidget.h \
    employeedialog.h \
    createdbwizardpage.h \
    appointmentwidget.h \
    appointmentrelationtablemodel.h \
    appointmentdialog.h \
    analysiswidget.h \
    adminwidget.h \
    admindbwizardpage.h \
    aboutdialog.h

OTHER_FILES += \
    clinic.pro.user \
    app.rc

FORMS += \
    aboutdialog.ui

RESOURCES += \
    clinic.qrc

RC_FILE = app.rc
