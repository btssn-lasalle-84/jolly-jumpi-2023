#include "IHM.h"
#include "statistiques.h"
#include "chevaux.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    chevaux w;
    w.showFullScreen();

    IHM ihm;

    ihm.jouer();

    return a.exec();
}
