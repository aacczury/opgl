#include "opgl.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    bool fs = false;
    QApplication a(argc, argv);

    opgl w( 0, fs );
    w.show();

    return a.exec();
}
