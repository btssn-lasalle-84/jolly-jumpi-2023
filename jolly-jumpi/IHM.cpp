/**
 * @file IHM.cpp
 *
 * @brief Définition de la classe IHM
 * @author Axel LAPIERRE
 * @version 0.1
 */

#include "IHM.h"
#include "ui_IHM.h"

/**
 * @brief Constructeur de la classe IHM
 *
 * @fn IHM::IHM
 * @param parent nullptr pour définir la fenêtre principale de l'application
 */

IHM::IHM(QWidget* parent) : QWidget(parent), ui(new Ui::IHM)
{
    qDebug() << Q_FUNC_INFO;

    instancierWidgets();

    initialiserWidgets();

    positionnerWidgets();

    connecterSignauxSlots();

    initialiserFenetre();

    afficherPageCourse();
}

/**
 * @brief Destructeur de la classe IHM
 *
 * @fn IHM::~IHM
 * @details Libère les ressources de l'application
 */

IHM::~IHM()
{
    delete ui;
}

void IHM::jouer()
{
}

/**
 * @fn IHM::afficherPage(IHM::Page page)
 * @brief Selectionne et affiche la page passée en paramètre
 */
void IHM::afficherPage(IHM::Page page)
{
    ui->pages->setCurrentIndex(page);
}

/**
 * @fn IHM::afficherPageConnexion()
 * @brief Affiche la page de connexion
 */
void IHM::afficherPageConnexion()
{
    afficherPage(IHM::Page::Connexion);
}

/**
 * @fn IHM::afficherCourse()
 * @brief Affiche la page pour une course de chevaux
 */
void IHM::afficherPageCourse()
{
    afficherPage(IHM::Page::Course);
}

void IHM::instancierWidgets()
{
    ui->setupUi(this);

    imageAvatarJoueur1 = new QPixmap("../Images/cheval.jpg");
    avatarJoueur1 = new QLabel(this);

    imageAvatarJoueur2 = new QPixmap("../Images/cheval.jpg");
    avatarJoueur2 = new QLabel(this);

    imageAvatarJoueur3 = new QPixmap("../Images/cheval.jpg");
    avatarJoueur3 = new QLabel(this);
}

void IHM::initialiserWidgets()
{
    avatarJoueur1->setPixmap(*imageAvatarJoueur1);
    ui->pages->widget(IHM::Page::Course)->layout()->addWidget(avatarJoueur1);

    avatarJoueur2->setPixmap(*imageAvatarJoueur2);
    ui->pages->widget(IHM::Page::Course)->layout()->addWidget(avatarJoueur2);

    avatarJoueur3->setPixmap(*imageAvatarJoueur3);
    ui->pages->widget(IHM::Page::Course)->layout()->addWidget(avatarJoueur3);
}

void IHM::positionnerWidgets()
{
}

void IHM::connecterSignauxSlots()
{
}

void IHM::initialiserFenetre()
{
#ifdef RASPBERRY_PI
    showFullScreen();
#else
    setFixedSize(qApp->desktop()->availableGeometry(this).width(),
           qApp->desktop()->availableGeometry(this).height());
    // showMaximized();
#endif
    afficherPageConnexion();
}
