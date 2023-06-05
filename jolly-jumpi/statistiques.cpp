#include "statistiques.h"
#include "course.h"

/**
 * @todo Initialiser les attributs
 */
Statistiques::Statistiques() : course(new class Course()), ihm(new class IHM()),
    nbChevaux(course->getNbChevaux()), classement(nbChevaux, 0), positionClassement(0),
    dureeDeLaPartie(0.0), nombreTirs(nbChevaux, 0), nombrePoints(nbChevaux, 0),
    joueurGagnant(AUCUN_JOUEUR)
{
}

Statistiques::~Statistiques()
{
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

void Statistiques::setNombreTirs(QVector<unsigned int> nombreTirs, int numeroCheval)
{
    this->nombreTirs[numeroCheval] = nombreTirs[numeroCheval]+1;
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
    if(positionClassement > course->getNbChevaux())
    {
        qDebug() << Q_FUNC_INFO;
        afficherResultats();
        positionClassement = 1;
        return joueurGagnant;
    }
    for(int numeroJoueur = 0; numeroJoueur < classement.size(); numeroJoueur++)
    {
        if(classement[positionClassement - 1] == positionChevaux2[numeroJoueur])
        {
            qDebug() << Q_FUNC_INFO << "numeroJoueur" << numeroJoueur;
            positionChevaux2[numeroJoueur] = DISTANCE_MAX + 1;
            return numeroJoueur;
        }
    }
    return AUCUN_JOUEUR;
}

void Statistiques::afficherResultats()
{
    qDebug() << Q_FUNC_INFO;
    afficherDureePartie();
    afficherPositionFinale(joueurGagnant);
    afficherNumeroJoueur(joueurGagnant);
    afficherPointsParSeconde(joueurGagnant);
    afficherNombrePointsParTir(joueurGagnant);
    positionClassement = 2;
    positionChevaux2 = course->getPositionChevaux(); //Copie pour sauvegarder les valeurs de positionChevaux sinon ça crash
}

void Statistiques::afficherResultatJoueurSuivant()
{
    int joueurSuivant = determinerJoueurSuivant();
    qDebug() << Q_FUNC_INFO << "positionClassement" << positionClassement
        << "joueurSuivant" << joueurSuivant;
    afficherPositionFinale(joueurSuivant);
    afficherClassement(positionClassement);
    afficherNumeroJoueur(joueurSuivant);
    afficherPointsParSeconde(joueurSuivant);
    afficherNombrePointsParTir(joueurSuivant);
    positionClassement++;
}
