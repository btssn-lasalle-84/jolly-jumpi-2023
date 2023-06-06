#include "statistiques.h"
#include "course.h"
#include "IHM.h"
#include <QDebug>
#include <algorithm>

Statistiques::Statistiques(IHM* ihm) :
    QObject(ihm), ihm(ihm), course(nullptr), nbChevaux(NB_CHEVAUX_MAX),
    classement(nbChevaux, 0), positionClassement(0), dureeDeLaPartie(0.0),
    nombreTirs(nbChevaux, 0), nombrePoints(nbChevaux, 0),
    joueurGagnant(AUCUN_JOUEUR)
{
    qDebug() << Q_FUNC_INFO;
}

Statistiques::~Statistiques()
{
    qDebug() << Q_FUNC_INFO;
}

void Statistiques::setCourse(Course* course)
{
    this->course = course;
    nbChevaux    = course->getNbChevaux();
}

int Statistiques::getJoueurGagnant() const
{
    return joueurGagnant;
}

QVector<unsigned int> Statistiques::getClassement() const
{
    return classement;
}

QVector<unsigned int> Statistiques::getNombreTirs() const
{
    return nombreTirs;
}

QVector<unsigned int> Statistiques::getNombrePoints() const
{
    return nombrePoints;
}

float Statistiques::getDureeDeLaPartie() const
{
    return dureeDeLaPartie;
}

void Statistiques::setJoueurGagnant(int joueurGagnant)
{
    this->joueurGagnant = joueurGagnant;
}

void Statistiques::setDureeDeLaPartie(float chronometre)
{
    this->dureeDeLaPartie = chronometre;
}

void Statistiques::setPositionClassement(int positionClassement)
{
    this->positionClassement = positionClassement;
}

void Statistiques::setNombreTirs(int numeroCheval)
{
    this->nombreTirs[numeroCheval] = nombreTirs[numeroCheval] + 1;
}

void Statistiques::setNombrePoints(int numeroCheval, int deplacement)
{
    this->nombrePoints[numeroCheval] = nombrePoints[numeroCheval] + deplacement;
}

void Statistiques::determinerClassement()
{
    QVector<unsigned int> positionChevaux = course->getPositionChevaux();
    if(joueurGagnant == AUCUN_JOUEUR)
        return;
    /**
     * @see QMap
     */
    classement.clear();
    classement = positionChevaux;
    std::sort(classement.begin(),
              classement.end(),
              std::greater<unsigned int>());
    positionClassement = 1;
    qDebug() << Q_FUNC_INFO << "classement" << classement;
    qDebug() << Q_FUNC_INFO << "joueurGagnant" << joueurGagnant;
}

int Statistiques::determinerJoueurSuivant()
{
    static QVector<unsigned int> copiePositionChevaux;

    if(positionClassement == 2)
    {
        copiePositionChevaux = course->getPositionChevaux();
    }
    else if(positionClassement > nbChevaux)
    {
        qDebug() << Q_FUNC_INFO;
        afficherResultats();
        positionClassement   = 1;
        copiePositionChevaux = course->getPositionChevaux();
        return joueurGagnant;
    }
    for(int numeroJoueur = 0; numeroJoueur < classement.size(); numeroJoueur++)
    {
        if(classement[positionClassement - 1] ==
           copiePositionChevaux[numeroJoueur])
        {
            qDebug() << Q_FUNC_INFO << "numeroJoueur" << numeroJoueur;
            copiePositionChevaux[numeroJoueur] = DISTANCE_MAX + 1;
            return numeroJoueur;
        }
    }
    return AUCUN_JOUEUR;
}

void Statistiques::afficherResultats()
{
    qDebug() << Q_FUNC_INFO << "joueurGagnant" << joueurGagnant;
    ihm->afficherDureePartie();
    ihm->afficherPositionFinale(joueurGagnant);
    ihm->afficherNumeroJoueur(joueurGagnant);
    ihm->afficherPointsParSeconde(joueurGagnant);
    ihm->afficherNombrePointsParTir(joueurGagnant);
    positionClassement = 2;
}

void Statistiques::afficherResultatJoueurSuivant()
{
    int joueurSuivant = determinerJoueurSuivant();
    qDebug() << Q_FUNC_INFO << "positionClassement" << positionClassement
             << "joueurSuivant" << joueurSuivant;
    ihm->afficherPositionFinale(joueurSuivant);
    ihm->afficherClassement(positionClassement);
    ihm->afficherNumeroJoueur(joueurSuivant);
    ihm->afficherPointsParSeconde(joueurSuivant);
    ihm->afficherNombrePointsParTir(joueurSuivant);
    positionClassement++;
}
