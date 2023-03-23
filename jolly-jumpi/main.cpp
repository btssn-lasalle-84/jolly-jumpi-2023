#include <QApplication>
#include "IHM.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    IHM          ihm;

    ihm.showFullScreen();

    return a.exec();
}
