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

IHM::IHM(QWidget* parent) :
    QWidget(parent), ui(new Ui::IHM), positionChevaux(NB_CHEVAUX_MAX, 0),
    nbChevaux(positionChevaux.size()), screen(QGuiApplication::primaryScreen()),
    screenGeometry(screen->availableGeometry().size())
{
    qDebug() << Q_FUNC_INFO << "nbChevaux" << nbChevaux;

    instancierWidgets();
    initialiserWidgets();
    positionnerWidgets();
    connecterSignauxSlots();
    initialiserFenetre();
#ifdef MODE_SIMULATION
    installerModeSimulation();
    afficherPageCourse();
#endif

    QSoundEffect musique;
    musique.setSource(
      QUrl::fromLocalFile("qrc:/musiques/Musiques/musique_de_fond_1.wav"));
    musique.setLoopCount(QSoundEffect::Infinite);
    musique.setVolume(1.0f);
    musique.play();
    qDebug() << Q_FUNC_INFO << "Musique joue ?" << musique.isPlaying();
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
 * @fn IHM::afficherPageCourse()
 * @brief Affiche la page pour une course de chevaux
 */
void IHM::afficherPageCourse()
{
    afficherPage(IHM::Page::Course);
}

void IHM::instancierWidgets()
{
    ui->setupUi(this);

    for(int i = 0; i < nbChevaux; i++)
    {
        imageAvatarsJoueurs.push_back(
          new QPixmap("Images/cheval" + QString::number(i + 1) + ".png"));
        avatarsJoueurs.push_back(new QLabel(this));

        imagePlaceHolder.push_back(new QPixmap("Images/placeholder.png"));
        placeHolder.push_back(new QLabel(this));
    }
}

void IHM::initialiserWidgets()
{
    for(int i = 0; i < imageAvatarsJoueurs.size(); i++)
    {
        *imageAvatarsJoueurs[i] = imageAvatarsJoueurs[i]->scaled(
          QSize(screenGeometry.width() * 0.1, screenGeometry.height() * 0.1));
        avatarsJoueurs[i]->setPixmap(*imageAvatarsJoueurs[i]);

        ui->pages->widget(IHM::Page::Course)
          ->findChild<QGridLayout*>("gridLayout")
          ->addWidget(avatarsJoueurs[i], i, 0, Qt::AlignTop);

        ui->pages->widget(IHM::Page::Course)
          ->findChild<QGridLayout*>("gridLayout")
          ->setRowStretch(imageAvatarsJoueurs.size(), 1);

        *imagePlaceHolder[i] = imagePlaceHolder[i]->scaled(
          QSize(screenGeometry.width() * 0.1, screenGeometry.height() * 0.1));
        placeHolder[i]->setPixmap(*imagePlaceHolder[i]);

        ui->pages->widget(IHM::Page::Course)
          ->findChild<QGridLayout*>("gridLayout")
          ->addWidget(placeHolder[i], nbChevaux, i, Qt::AlignTop);

        ui->pages->widget(IHM::Page::Course)
          ->findChild<QGridLayout*>("gridLayout")
          ->setRowStretch(imagePlaceHolder.size(), 1);
    }

    ui->pages->widget(IHM::Page::Course)
      ->findChild<QGridLayout*>("gridLayout")
      ->setRowStretch(imageAvatarsJoueurs.size(), 1);

    for(int i = 0; i <= DISTANCE_MAX; i++)
    {
        ui->pages->widget(IHM::Page::Course)
          ->findChild<QGridLayout*>("gridLayout")
          ->setColumnStretch(i, 1);
    }
}

void IHM::positionnerWidgets()
{
    ui->pages->widget(IHM::Page::Course)
      ->findChild<QGridLayout*>("gridLayout")
      ->setContentsMargins(0, screenGeometry.height() * 0.14, 0, 0);
}

void IHM::connecterSignauxSlots()
{
}

void IHM::initialiserFenetre()
{
#ifdef RASPBERRY_PI
    showFullScreen();
#else
    setFixedSize(screenGeometry.width(), screenGeometry.height());
    // showMaximized();
#endif

    afficherPageConnexion();
}

bool IHM::estPartieFinie()
{
    for(int i = 0; i < nbChevaux; i++)
    {
        if(positionChevaux[i] == DISTANCE_MAX)
        {
            qDebug() << Q_FUNC_INFO << "true";
            return true;
        }
    }
    qDebug() << Q_FUNC_INFO << "false";
    return false;
}

#ifdef MODE_SIMULATION
void IHM::installerModeSimulation()
{
    QAction* actionAvancementCheval = new QAction(this);
    actionAvancementCheval->setShortcut(QKeySequence(Qt::Key_Right));
    addAction(actionAvancementCheval);
    connect(actionAvancementCheval,
            SIGNAL(triggered()),
            this,
            SLOT(simulerAvancementCheval()));
}

int IHM::randInt(int min, int max)
{
    return ((QRandomGenerator::global()->bounded(min, max + 1)) + min);
}
#endif

void IHM::actualiserPositionChevaux(int numeroCheval, Trou deplacement)
{
    qDebug() << Q_FUNC_INFO << "Le cheval numéro" << numeroCheval + 1
             << ", qui était positionné à la case"
             << positionChevaux[numeroCheval] << "a avancé de"
             << int(deplacement);

    positionChevaux[numeroCheval] =
      positionChevaux[numeroCheval] + int(deplacement);
    if(positionChevaux[numeroCheval] > DISTANCE_MAX)
    {
        positionChevaux[numeroCheval] = DISTANCE_MAX;
    }
    qDebug() << Q_FUNC_INFO << "Il est maintenant case"
             << positionChevaux[numeroCheval];
    avancerChevaux();
}

void IHM::avancerChevaux()
{
    for(int i = 0; i < nbChevaux; i++)
    {
        avatarsJoueurs[i]->setPixmap(*imageAvatarsJoueurs[i]);
        ui->pages->widget(IHM::Page::Course)
          ->findChild<QGridLayout*>("gridLayout")
          ->removeWidget(avatarsJoueurs[i]);

        ui->pages->widget(IHM::Page::Course)
          ->findChild<QGridLayout*>("gridLayout")
          ->addWidget(avatarsJoueurs[i], i, positionChevaux[i], Qt::AlignTop);

        ui->pages->widget(IHM::Page::Course)
          ->findChild<QGridLayout*>("gridLayout")
          ->setRowStretch(imageAvatarsJoueurs.size(), 1);
    }
}

#ifdef MODE_SIMULATION
void IHM::simulerAvancementCheval()
{
    Trou trous[NB_COULEURS_TROU] = { JAUNE, BLEU, ROUGE };
    int  numeroCheval            = randInt(0, NB_CHEVAUX_MAX - 1);
    int  trou                    = randInt(0, NB_COULEURS_TROU - 1);
    actualiserPositionChevaux(numeroCheval, trous[trou]);
}
#endif
