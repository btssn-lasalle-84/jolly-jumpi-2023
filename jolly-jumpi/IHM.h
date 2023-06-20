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
#include "course.h"
#include "bluetooth.h"

/**
 * @def RASPBERRY_PI
 * @brief Pour le mode plein écran sur la RPI
 */
#define RASPBERRY_PI

#define MUSIQUE_DE_FOND ":/musiques/Musiques/musique_de_fond_1.wav"

namespace Ui
{
class IHM;
}

class Statistiques;
class Bluetooth;

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
        PageCourse,
        Parametres,
        ChangementParametres,
        PageStatistiques,
        NbPages
    };

    enum MenuAvantCourse
    {
        LancerPartie,
        AccederParametres,
        Quitter,
        NombreOptions
    };

    enum MenuStatistiques
    {
        QuitterStatistiques,
        JoueurSuivant,
        NbOptions
    };

    enum MenuParametres
    {
        ChangerNombreJoueurs,
        ChangerDureePartie,
        ChangerModeDeJeu,
        QuitterParametres,
        NbOptionsParametres
    };

    enum ChangerParametres
    {
        NombreJoueurs,
        Distance,
        ModeDeJeu
    };

  private:
    Course*       course;
    Statistiques* stats;
    Bluetooth*    bluetooth;
    bool          connecte;
    bool          estMenu;
    unsigned int  parametreSelectionne;

    Ui::IHM*          ui;
    QScreen*          screen;
    QSize             screenGeometry;
    QVector<QPixmap*> imageAvatarsJoueurs;
    QVector<QLabel*>  avatarsJoueurs;
    QVector<QPixmap*> imagePlaceHolder;
    QVector<QLabel*>  placeHolder;

    int   optionSelectionne;
    QFont police, policeStats, policeSelectionne;

    void instancierWidgets();
    void initialiserWidgets();
    void positionnerWidgets();
    void connecterSignauxSlots();
    void initialiserFenetre();
#ifdef MODE_SIMULATION
    void installerModeSimulation();
#endif
    void initialiserMusiqueDeFond();
    void deselectionner();
    void changerNombreJoueurs();
    void changerDureePartie();
    void changerModeDeJeu();

  public slots:
    void afficherPage(IHM::Page page);
    void afficherPageConnexion();
    void afficherPageAvantCourse();
    void afficherPageCourse();
    void afficherPageParametres();
    void afficherPageChangementParametre();
    void afficherPageStatistiques();
    void afficherResultatJoueurSuivant();
    void demarrerCourse();
    void ouvrirPageAvantCourse();
    void accederParametres();
    void quitterProgramme();
    void quitterStatistiques();
    void gererEtatConnexion();
    void gererEtatDeconnexion();
    void selectionnerSuivant();
    void selectionnerPrecedent();
    void changerSelection();
    void mettreEnEvidenceSelection();
    void validerSelection();
#ifdef MODE_SIMULATION
    void simulerAvancementCheval();
#endif

  public:
    IHM(QWidget* parent = nullptr);
    ~IHM();

    void jouer();
    void avancerChevaux();
    bool estBonIndex();
    void afficherDureePartie();
    void afficherClassement(int positionClassement);
    void afficherNombrePointsParTir(int numeroJoueur);
    void afficherPositionFinale(int numeroJoueur);
    void afficherPointsParSeconde(int numeroJoueur);
    void afficherNumeroJoueur(int numeroJoueur);
    bool estConnecte();
};

#endif // IHM_H
