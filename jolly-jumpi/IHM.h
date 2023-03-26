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

/**
 * @def RASPBERRY_PI
 * @brief Pour le mode plein écran sur la RPI
 */
//#define RASPBERRY_PI

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
        Course,
        NbPages
    };

  public:
    IHM(QWidget* parent = nullptr);
    ~IHM();

    void jouer();

  private:
    Ui::IHM* ui;

    void instancierWidgets();
    void initialiserWidgets();
    void positionnerWidgets();
    void connecterSignauxSlots();
    void initialiserFenetre();

  public slots:
    void afficherPage(IHM::Page page);
    void afficherPageConnexion();
    void afficherPageCourse();
};

#endif // IHM_H
