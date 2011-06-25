#include <QtGui/QApplication>
#include "mainwindow.h"
#include "globals.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setOrganizationName(Globals::OrganizationName);
    app.setOrganizationDomain(Globals::OrganizationDomainName);
    app.setApplicationName(Globals::ApplicationName);

//    QStringList libraries;
//    libraries << QCoreApplication::applicationDirPath() + "/plugins";
//    qApp->setLibraryPaths(libraries);

    MainWindow w;
    w.show();

    return app.exec();

}


