#ifndef CHEVAUX_H
#define CHEVAUX_H

#include <QWidget>

#include "IHM.h"

class chevaux : public IHM
{
    Q_OBJECT

public:
    chevaux(QWidget *parent = nullptr);
    ~chevaux();
    unsigned int avancerLesChevaux(unsigned int positionChevaux);

private:
    unsigned int positionChevaux;
};
#endif // CHEVAUX_H
