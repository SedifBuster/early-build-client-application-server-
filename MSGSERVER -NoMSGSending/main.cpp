#include "zmqserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    zmqServer w;
    w.show();

    return a.exec();
}
