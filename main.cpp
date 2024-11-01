#include "mainwindow.h"
#include <QFile>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile file("Combinear.qss");
    file.open(QFile::ReadOnly);
    QString style = QLatin1String(file.readAll());
    app.setStyleSheet(style);

    MainWindow w;
    w.show();
    return app.exec();
}
