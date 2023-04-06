#ifndef STATISTIQUES_H
#define STATISTIQUES_H

#include <QObject>

class Statistiques : public QObject
{
    Q_OBJECT

  private:
    unsigned int tempsDeLaPartie;
    unsigned int nombreBallesRentrees; //devrait etre un vecteur car plusieurs joueurs et plusieurs couleurs de trou

  public:
    Statistiques(QObject* parent = nullptr);
    ~Statistiques();
};

#endif // STATISTIQUES_H
