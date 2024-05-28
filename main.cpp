#include "mainwindow.h"
#include "calculation.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(1200,600);
    w.show();

    //Этот комментарий не имеет смыслового содержания

    return a.exec();
}
