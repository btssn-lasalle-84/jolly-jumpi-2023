#include "IHM.h"
#include "statistiques.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Statistiques w;
    w.show();
    return a.exec();
}
