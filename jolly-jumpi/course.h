#ifndef COURSE_H
#define COURSE_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include <QDebug>
#include <QLabel>

/**
 * @def MODE_SIMULATION
 * @brief Pour le mode simulation avec les touches de clavier
 */
#define MODE_SIMULATION

/**
 * @def NB_CHEVAUX_MAX
 * @brief Le nombre de chevaux maximum pour une course
 */
#define NB_CHEVAUX_MAX 6

/**
 * @def NB_COULEURS_TROU
 * @brief Le nombre de couleurs différentes pour les trous
 * @see Trou
 */
#define NB_COULEURS_TROU 3

/**
 * @def DISTANCE_MAX
 * @brief La distance max en nombre de cases
 */
#define DISTANCE_MAX 11

#define DISTANCE_MIN 6

/**
 * @def ATTENTE_FIN_COURSE
 * @brief La durée d'attente en ms à la fin d'une course
 */
#define ATTENTE_FIN_COURSE 5000

enum Trou
{
    JAUNE = 1,
    BLEU  = 3,
    ROUGE = 5
};

enum ModeDeJeu
{
    Normal,
    PointsAleatoire,
    NbModes
};

class Statistiques;
class IHM;
class Bluetooth;

class Course : public QObject
{
    Q_OBJECT

  private:
    IHM*          ihm;
    Statistiques* stats;
    Bluetooth*    bluetooth;

    int                   nbChevaux;
    int                   dureePartie;
    int                   numeroCheval;
    QVector<unsigned int> positionChevaux;
    QTimer*               timer;
    float                 chronometre;
    bool                  course;
    int                   modeDeJeu;

    void initialiserChronometre();
    bool estCourseFinie();
#ifdef MODE_SIMULATION
    int randInt(int min, int max);
#endif

  public:
    Course(IHM* ihm = 0);
    ~Course();

    void setStatistiques(Statistiques* stats);
    void setBluetooth(Bluetooth* bluetooth);
    void setNbChevaux(int nbChevaux);
    void setDureePartie(int dureePartie);
    void setModeDeJeu(int modeDeJeu);

    QVector<unsigned int> getPositionChevaux() const;
    int                   getNbChevaux() const;
    int                   getNbChevauxMax() const;
    int                   getDureePartie() const;
    int                   getDureeMax() const;
    int                   getDureeMin() const;
    void                  initialiserCourse();
#ifdef MODE_SIMULATION
    void simulerAvancementCheval();
#endif

  public slots:
    void chronometrer();
    void avancerChevaux();
    void actualiserPositionChevaux(int numeroCheval, int deplacement);
    void terminerCourse();
};

#endif // COURSE_H
