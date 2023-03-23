#ifndef CHEVAUX_H
#define CHEVAUX_H

#include <QWidget>
#include <QProgressBar>

#include "IHM.h"

#define LONGUEUR_PARTIE 100

class chevaux : public IHM
{
    Q_OBJECT

    private:
        QProgressBar *positionCheval1;
        QProgressBar *positionCheval2;
        QProgressBar *positionCheval3;

        const unsigned int longueurPartie;
        unsigned int positionChevaux;

    public:
        chevaux(QWidget *parent = nullptr);
        ~chevaux();
        void avancerLesChevaux(unsigned int positionChevaux);

};
#endif // CHEVAUX_H
