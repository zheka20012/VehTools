#include "VehTools.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VehTools w;
    w.show();
    return a.exec();
}
