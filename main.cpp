#include "mainwindow.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("greyMana.sqlite");

    db.open();
    if (!db.isOpen())
    {
       qDebug() << "Error: " << db.lastError().text();
    }

    else
    {
       qDebug() << "Successful connection to database";
    }


    return a.exec();
}
