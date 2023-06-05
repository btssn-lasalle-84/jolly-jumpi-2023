#ifndef COURSE_H
#define COURSE_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include <QDebug>
#include <QLabel>

#include "IHM.h"
#include "Statistiques.h"

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
#define DISTANCE_MAX 10

/**
 * @def ATTENTE_FIN_COURSE
 * @brief La durée d'attente en ms à la fin d'une course
 */
#define ATTENTE_FIN_COURSE 5000

enum Trou
{
    JAUNE = 1,
    BLEU = 3,
    ROUGE = 5
};

class Course : public IHM
{
    Q_OBJECT

private:
    class Statistiques *stats;
    class IHM *ihm;

    int nbChevaux;
    int numeroCheval;
    QVector<unsigned int> positionChevaux;
    QTimer *timer;
    float chronometre;
    bool course;

    void initialiserChronometre();
    bool estCourseFinie();
    void terminerCourse();
#ifdef MODE_SIMULATION
    int randInt(int min, int max);
#endif

public:
    Course();
    ~Course();

    QVector<unsigned int> getPositionChevaux() const;
    int getNbChevaux() const;
    void initialiserCourse();
#ifdef MODE_SIMULATION
    void simulerAvancementCheval();
#endif

public slots:
    void chronometrer();
    void avancerChevaux();

    void actualiserPositionChevaux(int numeroCheval, Trou deplacement);
};

#endif // COURSE_H
