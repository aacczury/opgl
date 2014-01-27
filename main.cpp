#include "opgl.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    opgl w;
    w.show();

    return a.exec();
}
