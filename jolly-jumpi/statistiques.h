#ifndef STATISTIQUES_H
#define STATISTIQUES_H

#include <QObject>

class Statistiques : public QObject
{
    Q_OBJECT

  private:
    unsigned int tempsDeLaPartie;
    /**
     * @todo Implémenter la notion de conteneur
     */
    unsigned int nombreBallesRentrees;

  public:
    Statistiques(QObject* parent = nullptr);
    ~Statistiques();
};

#endif // STATISTIQUES_H
