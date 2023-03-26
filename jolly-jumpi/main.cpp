#include <QApplication>
#include "IHM.h"

/**
 * @file main.cpp
 * @brief Programme principal
 * @details Crée et affiche la fenêtre principale de l'application JollyJumpi
 * @author Axel LAPIERRE
 * @version 1.0
 *
 * @param argc
 * @param argv[]
 * @return int
 */
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    IHM          ihm;

    ihm.show();

    return a.exec();
}
