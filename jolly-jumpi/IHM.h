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

  private:
    class Course *course;
    class Statistiques *stats;

    Ui::IHM*              ui;
    QScreen*              screen;
    QSize                 screenGeometry;
    QVector<QPixmap *> imageAvatarsJoueurs;
    QVector<QLabel *> avatarsJoueurs;
    QVector<QPixmap *> imagePlaceHolder;
    QVector<QLabel *> placeHolder;

    void instancierWidgets();
    void initialiserWidgets();
    void positionnerWidgets();
    void connecterSignauxSlots();
    void initialiserFenetre();
#ifdef MODE_SIMULATION
    void installerModeSimulation();
#endif
    void initialiserMusiqueDeFond();

  public slots:
    void afficherPage(IHM::Page page);
    void afficherPageConnexion();
    void afficherPageAvantCourse();
    void afficherPageCourse();
    void afficherPageParametres();
    void afficherPageStatistiques();
    void afficherResultatJoueurSuivant();
    void demarrerCourse();
    void ouvrirPageAvantCourse();
    void accederParametres();
    void quitterProgramme();
    void quitterStatistiques();
    void simulerAvancementCheval();

  public:
    IHM(QWidget* parent = nullptr);
    ~IHM();

    void jouer();
    void avancerChevaux();
    bool estBonIndex();

    void afficherDureePartie();
    void afficherClassement(int positionClassement);
    void afficherNombrePointsParTir(unsigned int numeroJoueur);
    void afficherPositionFinale(int numeroJoueur);
    void afficherPointsParSeconde(int numeroJoueur);
    void afficherNumeroJoueur(int numeroJoueur);
};

#endif // IHM_H
