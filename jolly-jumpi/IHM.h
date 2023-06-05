#ifndef IHM_H
#define IHM_H

/**
 * @file IHM.h
 *
 * @brief Déclaration de la classe IHM
 * @author Axel LAPIERRE
 * @version 0.1
 */

#include <QtWidgets>
#include <QApplication>
#include <QScreen>
#include <QRandomGenerator>
#include <QSoundEffect>
#include <QTimer>
#include <algorithm>

/**
 * @def RASPBERRY_PI
 * @brief Pour le mode plein écran sur la RPI
 */
//#define RASPBERRY_PI

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
#define DISTANCE_MAX 10

/**
 * @def ATTENTE_FIN_COURSE
 * @brief La durée d'attente en ms à la fin d'une course
 */
#define ATTENTE_FIN_COURSE 5000

#define AUCUN_JOUEUR -1

#define MUSIQUE_DE_FOND ":/musiques/Musiques/musique_de_fond_1.wav"

namespace Ui
{
class IHM;
}

/**
 * @class IHM
 * @brief Affiche les pages d'écran pour l'application
 */
class IHM : public QWidget
{
    Q_OBJECT
    /**
     * @enum Page
     * @brief Les différentes pages de la GUI
     */
    enum Page
    {
        Connexion,
        AvantCourse,
        Course,
        Statistiques,
        Parametres,
        NbPages
    };
    enum Trou
    {
        JAUNE = 1,
        BLEU  = 3,
        ROUGE = 5
    };

  private:
    Ui::IHM*              ui;
    int                   nbChevaux;
    QVector<unsigned int> positionChevaux;
    QVector<unsigned int> positionChevaux2; //Pour le calcul de position dans les stats
    QVector<unsigned int> classement;
    int                   joueurGagnant;
    int                   positionClassement;
    QVector<QPixmap*>     imageAvatarsJoueurs;
    QVector<QLabel*>      avatarsJoueurs;
    QVector<QPixmap*>     imagePlaceHolder;
    QVector<QLabel*>      placeHolder;
    QScreen*              screen;
    QSize                 screenGeometry;
    QTimer*               timer;
    float                 chronometre;
    float                 dureeDeLaPartie;
    QVector<unsigned int> nombreTirs;
    QVector<unsigned int> nombrePoints;
    bool                  course;

    void instancierWidgets();
    void initialiserWidgets();
    void positionnerWidgets();
    void initialiserChronometre();
    void connecterSignauxSlots();
    void initialiserFenetre();
    void initialiserMusiqueDeFond();
    void initialiserCourse();
    bool estCourseFinie();
    void terminerCourse();
    void determinerClassement();
    int  determinerJoueurSuivant();
    void afficherResultats();
    void afficherDureePartie();
    void afficherClassement(int joueurSuivant);
    void afficherPositionFinale(int numeroJoueur);
    void afficherNumeroJoueur(int numeroJoueur);
    void afficherPointsParSeconde(int numeroJoueur);
    void afficherNombrePointsParTir(int numeroJoueur);
    void afficherGagnant();
#ifdef MODE_SIMULATION
    void installerModeSimulation();
    int  randInt(int min, int max);
#endif

  public slots:
    void afficherPage(IHM::Page page);
    void afficherPageConnexion();
    void afficherPageAvantCourse();
    void afficherPageCourse();
    void afficherPageParametres();
    void afficherPageStatistiques();
    void afficherResultatJoueurSuivant();
    void chronometrer();
    void actualiserPositionChevaux(int numeroCheval, Trou deplacement);
    void demarrerCourse();
    void ouvrirPageAvantCourse();
    void accederParametres();
    void quitterProgramme();
    void quitterStatistiques();
    void avancerChevaux();
    void attendreFinCourse();
#ifdef MODE_SIMULATION
    void simulerAvancementCheval();
#endif

  public:
    IHM(QWidget* parent = nullptr);
    ~IHM();

    void jouer();
};

#endif // IHM_H
