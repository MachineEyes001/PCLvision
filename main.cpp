#include "PCLvision.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PCLvision w;
    w.show();
    return a.exec();
}
