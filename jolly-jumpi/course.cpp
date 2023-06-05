#include "course.h"
#include "statistiques.h"

Course::Course() : stats(new class Statistiques()), ihm(new class IHM()),
    nbChevaux(NB_CHEVAUX_MAX), numeroCheval(0), positionChevaux(nbChevaux, 0),
    chronometre(0.0), course(false)
{
}

Course::~Course()
{
    delete stats;
}

QVector<unsigned int> Course::getPositionChevaux() const
{
    return positionChevaux;
}

int Course::getNbChevaux() const
{
    return nbChevaux;
}

void Course::initialiserChronometre()
{
    timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(chronometrer()));
}

void Course::chronometrer()
{
    chronometre += 0.01;
}

void Course::initialiserCourse()
{
    stats->setJoueurGagnant(AUCUN_JOUEUR);
    stats->setPositionClassement(0);
    for (int i = 0; i < nbChevaux; i++)
    {
        positionChevaux[i] = 0;
    }
    timer->start();
    avancerChevaux();
    course = true;
}

bool Course::estCourseFinie()
{
    for (int i = 0; i < nbChevaux; i++)
    {
        if (positionChevaux[i] == DISTANCE_MAX)
        {
            stats->setJoueurGagnant(i);
            qDebug() << Q_FUNC_INFO << "DISTANCE_MAX"
                     << "true"
                     << "joueurGagnant" << stats->getJoueurGagnant();
            return true;
        }
    }
    qDebug() << Q_FUNC_INFO << "DISTANCE_MAX"
             << "false";
    return false;
}

void Course::terminerCourse()
{
    course = false;
    stats->setDureeDeLaPartie(chronometre);
    stats->determinerClassement();
    stats->afficherResultats();
    timer->stop();
    QTimer::singleShot(ATTENTE_FIN_COURSE, this, SLOT(ihm->afficherPageStatistiques()));
    chronometre = 0;
}

void Course::avancerChevaux()
{
    ihm->avancerChevaux();
    if (estCourseFinie())
        terminerCourse();
}

void Course::actualiserPositionChevaux(int numeroCheval, Trou deplacement)
{
    if (!course)
        return;
    QVector<unsigned int> nombreTirs = stats->getNombreTirs();
    QVector<unsigned int> nombrePoints = stats->getNombreTirs();
    stats->setNombreTirs(nombreTirs, numeroCheval);
    nombrePoints[numeroCheval] += deplacement;
    if (nombrePoints[numeroCheval] >= DISTANCE_MAX)
        nombrePoints[numeroCheval] = DISTANCE_MAX;
    qDebug() << Q_FUNC_INFO << "Le cheval numéro" << numeroCheval + 1
             << ", qui était positionné à la case"
             << positionChevaux[numeroCheval] << "a avancé de"
             << int(deplacement);
    qDebug() << Q_FUNC_INFO << "chronometre" << chronometre;

    qDebug() << Q_FUNC_INFO << "nombreTirs" << nombreTirs[numeroCheval]
             << "nombrePoints" << nombrePoints[numeroCheval];

    positionChevaux[numeroCheval] =
        positionChevaux[numeroCheval] + int(deplacement);
    if (positionChevaux[numeroCheval] > DISTANCE_MAX)
    {
        positionChevaux[numeroCheval] = DISTANCE_MAX;
    }

    qDebug() << Q_FUNC_INFO << "Il est maintenant case"
             << positionChevaux[numeroCheval];
    avancerChevaux();
}

#ifdef MODE_SIMULATION
void Course::simulerAvancementCheval()
{
    if (ihm->estBonIndex())
    {
        Trou trous[NB_COULEURS_TROU] = {JAUNE, BLEU, ROUGE};
        int numeroCheval = randInt(0, NB_CHEVAUX_MAX - 1);
        int trou = randInt(0, NB_COULEURS_TROU - 1);
        actualiserPositionChevaux(numeroCheval, trous[trou]);
    }
}

int Course::randInt(int min, int max)
{
    return ((QRandomGenerator::global()->bounded(min, max + 1)) + min);
}
#endif
