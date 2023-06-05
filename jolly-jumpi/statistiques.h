#ifndef STATISTIQUES_H
#define STATISTIQUES_H

#include <QObject>
#include <QVector>
#include <qDebug>
#include "IHM.h"

#define AUCUN_JOUEUR -1

class Statistiques : public IHM
{
    Q_OBJECT

  private:
    class Course *course;
    class IHM *ihm;

    int                   nbChevaux;
    QVector<unsigned int> positionChevaux2; //Pour le calcul de position dans les stats
    QVector<unsigned int> classement;
    int                   positionClassement;
    float                 dureeDeLaPartie;
    QVector<unsigned int> nombreTirs;
    QVector<unsigned int> nombrePoints;
    int                   joueurGagnant;

    int determinerJoueurSuivant();

  public:
    Statistiques();
    ~Statistiques();

    int getJoueurGagnant() const;
    QVector<unsigned int> getClassement() const;
    QVector<unsigned int> getNombreTirs() const;
    QVector<unsigned int> getNombrePoints() const;
    float getDureeDeLaPartie() const;

    void setJoueurGagnant(int);
    void setPositionClassement(int);
    void setDureeDeLaPartie(float);
    void setNombreTirs(QVector<unsigned int>, int);

    void determinerClassement();
    void afficherResultats();
    void afficherResultatJoueurSuivant();
};

#endif // STATISTIQUES_H
