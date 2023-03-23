#ifndef STATISTIQUES_H
#define STATISTIQUES_H

#include <QWidget>

#include "IHM.h"

class Statistiques : public IHM
{
    Q_OBJECT

private:
    unsigned int temps;
    unsigned int nombreBallesRentrees;

public:
    Statistiques(QWidget *parent = nullptr);
    ~Statistiques();
};

#endif // STATISTIQUES_H
