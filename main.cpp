#include "sistema_scada.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sistema_scada w;
    w.show();

    return a.exec();
}
