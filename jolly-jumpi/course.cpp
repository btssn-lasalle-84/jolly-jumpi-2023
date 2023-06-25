#include "course.h"
#include "IHM.h"
#include "statistiques.h"

Course::Course(IHM* ihm) :
    QObject(ihm), ihm(ihm), stats(nullptr), bluetooth(nullptr),
    nbChevaux(NB_CHEVAUX_MAX), dureePartie(DISTANCE_MAX), numeroCheval(0),
    positionChevaux(nbChevaux, 0), chronometre(-3.0), course(false), modeDeJeu(0)
{
    qDebug() << Q_FUNC_INFO;
    initialiserChronometre();
}

Course::~Course()
{
    qDebug() << Q_FUNC_INFO;
}

void Course::setStatistiques(Statistiques* stats)
{
    this->stats = stats;
}

void Course::setBluetooth(Bluetooth* bluetooth)
{
    this->bluetooth = bluetooth;
}

void Course::setNbChevaux(int nbChevaux)
{
    this->nbChevaux = nbChevaux;
}

void Course::setDureePartie(int dureePartie)
{
    this->dureePartie = dureePartie;
}

void Course::setModeDeJeu(int modeDeJeu)
{
    this->modeDeJeu = modeDeJeu;
}

QVector<unsigned int> Course::getPositionChevaux() const
{
    return positionChevaux;
}

int Course::getNbChevaux() const
{
    return nbChevaux;
}

int Course::getNbChevauxMax() const
{
    return NB_CHEVAUX_MAX;
}

int Course::getDureePartie() const
{
    return dureePartie;
}

int Course::getDureeMax() const
{
    return DISTANCE_MAX;
}

int Course::getDureeMin() const
{
    return DISTANCE_MIN;
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
    for(int i = 0; i < nbChevaux; i++)
    {
        positionChevaux[i] = 0;
    }
    timer->start();
    avancerChevaux();
    course = true;
    if(modeDeJeu == ModeDeJeu::Normal)
        bluetooth->envoyerTrameDebutCourse();
    if(modeDeJeu == ModeDeJeu::PointsAleatoire)
        bluetooth->envoyerTrameDebutCourseAleatoire();
    qDebug() << Q_FUNC_INFO << "dureePartie" << dureePartie << "nbChevaux"
             << nbChevaux << "modeDeJeu" << modeDeJeu;
}

bool Course::estCourseFinie()
{
    for(int i = 0; i < nbChevaux; i++)
    {
        if(positionChevaux[i] == dureePartie)
        {
            stats->setJoueurGagnant(i);
            qDebug() << Q_FUNC_INFO << "dureePartie"
                     << "true"
                     << "joueurGagnant" << stats->getJoueurGagnant();
            return true;
        }
    }
    qDebug() << Q_FUNC_INFO << "dureePartie"
             << "false";
    return false;
}

void Course::terminerCourse()
{
    course = false;
    if(bluetooth->getAbandon() == false)
    {
        stats->setDureeDeLaPartie(chronometre);
        if(stats->getRecord() > chronometre)
        {
            qDebug() << Q_FUNC_INFO << "record" << stats->getRecord()
                     << "chronometre" << chronometre;
            stats->setRecord(chronometre);
        }
        stats->determinerClassement();
        QTimer::singleShot(ATTENTE_FIN_COURSE,
                           ihm,
                           SLOT(afficherPageStatistiques()));
        stats->afficherResultats();
    }
    else
    {
        ihm->afficherPageConnexion();
        bluetooth->setAbandon(false);
    }
    timer->stop();
    chronometre = 0;
    qDebug() << Q_FUNC_INFO;
    bluetooth->envoyerTrameFinCourse();
}

void Course::avancerChevaux()
{
    ihm->avancerChevaux();
    if(estCourseFinie())
        terminerCourse();
}

void Course::actualiserPositionChevaux(int numeroCheval, int deplacement)
{
    if(!course)
        return;

    enum Trou
    {
        JAUNE = 1,
        BLEU  = 3,
        ROUGE = 5
    };

    switch(deplacement)
    {
        case 0:
            deplacement = Trou::JAUNE;
            break;
        case 1:
            deplacement = Trou::BLEU;
            break;
        case 2:
            deplacement = Trou::ROUGE;
            break;
        default:
            break;
    }

    if(modeDeJeu == ModeDeJeu::PointsAleatoire)
    {
        deplacement = randInt(0, 5);
    }

    qDebug() << Q_FUNC_INFO << "deplacement" << deplacement;

    stats->setNombreTirs(numeroCheval);
    stats->setNombrePoints(numeroCheval, deplacement);
    QVector<unsigned int> nombreTirs   = stats->getNombreTirs();
    QVector<unsigned int> nombrePoints = stats->getNombrePoints();
    if(nombrePoints[numeroCheval] >= dureePartie)
        nombrePoints[numeroCheval] = dureePartie;
    qDebug() << Q_FUNC_INFO << "Le cheval numéro" << numeroCheval + 1
             << ", qui était positionné à la case"
             << positionChevaux[numeroCheval] << "a avancé de"
             << int(deplacement);
    qDebug() << Q_FUNC_INFO << "chronometre" << chronometre;

    qDebug() << Q_FUNC_INFO << "nombreTirs" << nombreTirs[numeroCheval]
             << "nombrePoints" << nombrePoints[numeroCheval];

    positionChevaux[numeroCheval] =
      positionChevaux[numeroCheval] + int(deplacement);
    if(positionChevaux[numeroCheval] > dureePartie)
    {
        positionChevaux[numeroCheval] = dureePartie;
    }

    qDebug() << Q_FUNC_INFO << "Il est maintenant case"
             << positionChevaux[numeroCheval];
    avancerChevaux();
}

#ifdef MODE_SIMULATION
void Course::simulerAvancementCheval()
{
    if(ihm->estBonIndex())
    {
        Trou trous[NB_COULEURS_TROU] = { JAUNE, BLEU, ROUGE };
        int  numeroCheval            = randInt(0, nbChevaux - 1);
        int  trou                    = randInt(0, NB_COULEURS_TROU - 1);
        actualiserPositionChevaux(numeroCheval, trous[trou]);
    }
}

int Course::randInt(int min, int max)
{
    return ((QRandomGenerator::global()->bounded(min, max + 1)) + min);
}
#endif
