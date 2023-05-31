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

//#include <QAudioDevice>
//#include <QMediaDevices>
/*#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFile>
#include <QAudioFormat>
#include <QIODevice>
#include <QSound>
#include <QMediaPlaylist>*/

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
    QVector<unsigned int> positionChevaux;
    int                   nbChevaux;
    QVector<QPixmap*>     imageAvatarsJoueurs;
    QVector<QLabel*>      avatarsJoueurs;
    QVector<QPixmap*>     imagePlaceHolder;
    QVector<QLabel*>      placeHolder;
    QScreen*              screen;
    QSize                 screenGeometry;

    void instancierWidgets();
    void initialiserWidgets();
    void positionnerWidgets();
    void connecterSignauxSlots();
    void initialiserFenetre();
    void initialiserMusiqueDeFond();
    bool estPartieFinie();
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
    void actualiserPositionChevaux(int numeroCheval, Trou deplacement);
    void demarrerCourse();
    void ouvrirPageAvantCourse();
    void accederParametres();
    void quitterProgramme();
    void avancerChevaux();
#ifdef MODE_SIMULATION
    void simulerAvancementCheval();
#endif

  public:
    IHM(QWidget* parent = nullptr);
    ~IHM();

    void jouer();
};

#endif // IHM_H
