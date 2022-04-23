#include <QCoreApplication>
#include "mainclass.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MainClass ffff;
    ffff.init_date();
    ffff.transit_to_can();
    ffff.check_origin_basis();
    ffff.simplex_method();
    return a.exec();
}
