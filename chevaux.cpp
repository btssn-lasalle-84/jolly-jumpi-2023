#include "chevaux.h"

chevaux::chevaux(QWidget *parent):
    longueurPartie(LONGUEUR_PARTIE),
    positionChevaux(0)
{
    positionCheval1 = new QProgressBar(this);
    positionCheval1->setRange(0, LONGUEUR_PARTIE);
    positionCheval1->setValue(0);
    positionCheval1->setGeometry(10, 10, 200, 30);

    positionCheval2 = new QProgressBar(this);
    positionCheval2->setRange(0, LONGUEUR_PARTIE);
    positionCheval2->setValue(0);
    positionCheval2->setGeometry(10, 100, 200, 30);

    positionCheval3 = new QProgressBar(this);
    positionCheval3->setRange(0, LONGUEUR_PARTIE);
    positionCheval3->setValue(0);
    positionCheval3->setGeometry(10, 200, 200, 30);
}

void chevaux::avancerLesChevaux(unsigned int positionChevaux)
{
    positionChevaux = 15;
    this->positionChevaux = positionChevaux;
    positionCheval1->setValue(15);
    positionCheval2->setValue(positionChevaux);
    positionCheval3->setValue(positionChevaux);
}

chevaux::~chevaux()
{
}
