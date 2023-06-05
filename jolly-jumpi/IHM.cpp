/**
 * @file IHM.cpp
 *
 * @brief Définition de la classe IHM
 * @author Axel LAPIERRE
 * @version 0.1
 */

#include "course.h"
#include "statistiques.h"
#include "IHM.h"
#include "ui_IHM.h"

/**
 * @brief Constructeur de la classe IHM
 *
 * @fn IHM::IHM
 * @param parent nullptr pour définir la fenêtre principale de l'application
 */

IHM::IHM(QWidget* parent) :
    QWidget(parent), course(new class Course()),
    stats(new class Statistiques()), ui(new Ui::IHM),
    screen(QGuiApplication::primaryScreen()),
    screenGeometry(screen->availableGeometry().size())
{
    instancierWidgets();
    initialiserWidgets();
    positionnerWidgets();
    initialiserFenetre();
#ifdef MODE_SIMULATION
    installerModeSimulation();
#endif
    initialiserMusiqueDeFond();
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

void IHM::afficherPageParametres()
{
    afficherPage(IHM::Page::Parametres);
}

void IHM::afficherPageAvantCourse()
{
    afficherPage(IHM::Page::AvantCourse);
}

void IHM::afficherPageStatistiques()
{
    afficherPage(IHM::Page::Statistiques);
}

void IHM::instancierWidgets()
{
    ui->setupUi(this);
    qDebug() << Q_FUNC_INFO;
    for(int i = 0; i < course->getNbChevaux(); i++)
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
    qDebug() << Q_FUNC_INFO;
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
          ->addWidget(placeHolder[i], course->getNbChevaux(), i, Qt::AlignTop);

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
    qDebug() << Q_FUNC_INFO;
    ui->pages->widget(IHM::Page::Course)
      ->findChild<QGridLayout*>("gridLayout")
      ->setContentsMargins(0, screenGeometry.height() * 0.14, 0, 0);
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

void IHM::initialiserMusiqueDeFond()
{
    QSoundEffect musique;
    musique.setSource(QUrl::fromLocalFile(MUSIQUE_DE_FOND));
    musique.setLoopCount(QSoundEffect::Infinite);
    musique.setVolume(1.0f);
    musique.play();
    qDebug() << Q_FUNC_INFO << "isPlaying" << musique.isPlaying();
}

void IHM::demarrerCourse()
{
    if(ui->pages->currentIndex() == IHM::Page::AvantCourse)
    {
        course->initialiserCourse();
        afficherPageCourse();
    }
}

void IHM::ouvrirPageAvantCourse()
{
    if(ui->pages->currentIndex() == IHM::Page::Connexion)
    {
        afficherPageAvantCourse();
    }
}

void IHM::accederParametres()
{
    if(ui->pages->currentIndex() == IHM::Page::AvantCourse)
    {
        afficherPageParametres();
    }
}

void IHM::quitterProgramme()
{
    static QVector<unsigned int> copiePositionChevaux;

    if(positionClassement == 2)
    {
        copiePositionChevaux = positionChevaux;
    }
    else if(positionClassement > nbChevaux)
    {
        qDebug() << Q_FUNC_INFO;
        afficherResultats();
        positionClassement   = 1;
        copiePositionChevaux = positionChevaux;
        return joueurGagnant;
    }
    for(int numeroJoueur = 0; numeroJoueur < classement.size(); numeroJoueur++)
    {
        if(classement[positionClassement - 1] ==
           copiePositionChevaux[numeroJoueur])
        {
            qDebug() << Q_FUNC_INFO << "numeroJoueur" << numeroJoueur;
            copiePositionChevaux[numeroJoueur] = DISTANCE_MAX + 1;
            return numeroJoueur;
        }
    }
}

void IHM::quitterStatistiques()
{
    qDebug() << Q_FUNC_INFO << "joueurGagnant" << joueurGagnant;
    afficherDureePartie();
    afficherPositionFinale(joueurGagnant);
    afficherNumeroJoueur(joueurGagnant);
    afficherPointsParSeconde(joueurGagnant);
    afficherNombrePointsParTir(joueurGagnant);
    positionClassement = 2;
}

void IHM::afficherResultatJoueurSuivant()
{
    if(ui->pages->currentIndex() == IHM::Page::Statistiques)
        stats->afficherResultatJoueurSuivant();
}

void IHM::avancerChevaux()
{
    QVector<unsigned int> positionChevaux = course->getPositionChevaux();
    QVector<unsigned int> classement = stats->getClassement();
    for (int i = 0; i < course->getNbChevaux(); i++)
{
    avatarsJoueurs[i]->setPixmap(*imageAvatarsJoueurs[i]);
    ui->pages->widget(IHM::Page::Course)
        ->findChild<QGridLayout *>("gridLayout")
        ->removeWidget(avatarsJoueurs[i]);

    ui->pages->widget(IHM::Page::Course)
        ->findChild<QGridLayout *>("gridLayout")
        ->addWidget(avatarsJoueurs[i], i, positionChevaux[i], Qt::AlignTop);

    ui->pages->widget(IHM::Page::Course)
        ->findChild<QGridLayout *>("gridLayout")
        ->setRowStretch(imageAvatarsJoueurs.size(), 1);

    if (positionChevaux[i] == DISTANCE_MAX)
        classement.push_back(i);
    }
}

bool IHM::estBonIndex()
{
    if(ui->pages->currentIndex() == IHM::Course && course)
        return true;
    return false;
}

void IHM::afficherDureePartie()
{
    float dureeDeLaPartie = stats->getDureeDeLaPartie();
    ui->pages->widget(IHM::Page::Statistiques)
        ->findChild<QLabel*>("duree")
        ->setText(QString::number(dureeDeLaPartie, 'f', 2) + " secondes");
    qDebug() << Q_FUNC_INFO << "dureeDeLaPartie" << dureeDeLaPartie;
}

void IHM::afficherClassement(int positionClassement)
{
    ui->pages->widget(IHM::Page::Statistiques)
        ->findChild<QLabel*>("duree")
        ->setText(QString::number(positionClassement) + "e position");
}

void IHM::afficherNombrePointsParTir(unsigned int numeroJoueur)
{
    if(numeroJoueur == AUCUN_JOUEUR)
        return;
    unsigned int pointsParTir = 0;
    QVector<unsigned int> positionChevaux = course->getPositionChevaux();
    QVector<unsigned int> nombrePoints = stats->getNombrePoints();
    QVector<unsigned int> nombreTirs = stats->getNombreTirs();
    if(positionChevaux[numeroJoueur] != 0)
    {
        qDebug() << Q_FUNC_INFO << "numeroJoueur" << numeroJoueur
                 << "nombrePoints" << nombrePoints[numeroJoueur] << "nombreTirs"
                 << nombreTirs[numeroJoueur];
        pointsParTir = (nombrePoints[numeroJoueur] / nombreTirs[numeroJoueur]);
        qDebug() << Q_FUNC_INFO << "numeroJoueur" << numeroJoueur
                 << "pointsParTir" << pointsParTir;
        ui->pages->widget(IHM::Page::Statistiques)
            ->findChild<QLabel*>("ppt")
            ->setText(QString::number(pointsParTir) + " points par tir");
    }
    else
    {
        ui->pages->widget(IHM::Page::Statistiques)
            ->findChild<QLabel*>("ppt")
            ->setText("0 point par tir");
    }
}

void IHM::afficherPositionFinale(int numeroJoueur)
{
    if(numeroJoueur == AUCUN_JOUEUR)
        return;
    QVector <unsigned int> positionChevaux = course->getPositionChevaux();
    unsigned int pourcentageCompletion =
        (positionChevaux[numeroJoueur] * 100) / DISTANCE_MAX;
    qDebug() << Q_FUNC_INFO << "numeroJoueur" << numeroJoueur
             << "pourcentageCompletion" << pourcentageCompletion;
    ui->pages->widget(IHM::Page::Statistiques)
        ->findChild<QLabel*>("temps")
        ->setText("Course finie à " + QString::number(pourcentageCompletion) +
                  "%");
}

void IHM::afficherPointsParSeconde(int numeroJoueur)
{
    if(numeroJoueur == AUCUN_JOUEUR)
        return;
    float pointsParSeconde = 0.0;
    QVector<unsigned int> positionChevaux = course->getPositionChevaux();
    QVector<unsigned int> nombrePoints = stats->getNombrePoints();
    float dureeDeLaPartie = stats->getDureeDeLaPartie();
    if(positionChevaux[numeroJoueur] != 0)
    {
        qDebug() << Q_FUNC_INFO << "positionChevaux[numeroJoueur]"
                 << positionChevaux[numeroJoueur];
        pointsParSeconde = (nombrePoints[numeroJoueur] / dureeDeLaPartie);
        qDebug() << Q_FUNC_INFO << "numeroJoueur" << numeroJoueur
                 << "pointsParSeconde" << pointsParSeconde;
        ui->pages->widget(IHM::Page::Statistiques)
            ->findChild<QLabel*>("pps")
            ->setText(QString::number(pointsParSeconde, 'f', 2) +
                      " points par seconde");
    }
    else
    {
        ui->pages->widget(IHM::Page::Statistiques)
            ->findChild<QLabel*>("pps")
            ->setText("0 point par seconde");
    }
}

void IHM::afficherNumeroJoueur(int numeroJoueur)
{
    if(numeroJoueur == AUCUN_JOUEUR)
        return;
    if(numeroJoueur == stats->getJoueurGagnant())
        ui->pages->widget(IHM::Page::Statistiques)
            ->findChild<QLabel*>("position")
            ->setText("Gagnant : joueur " + QString::number(stats->getJoueurGagnant() + 1));
    else
    {
        ui->pages->widget(IHM::Page::Statistiques)
            ->findChild<QLabel*>("position")
            ->setText("Joueur " + QString::number(numeroJoueur + 1));
    }
}

void IHM::simulerAvancementCheval()
{
    course->simulerAvancementCheval();
}

#ifdef MODE_SIMULATION
void IHM::installerModeSimulation()
{
    qDebug() << Q_FUNC_INFO;
    QAction* actionAvancementCheval = new QAction(this);
    actionAvancementCheval->setShortcut(QKeySequence(Qt::Key_Right));
    addAction(actionAvancementCheval);
    connect(actionAvancementCheval,
            SIGNAL(triggered()),
            this,
            SLOT(simulerAvancementCheval()));

    QAction* demarrage = new QAction(this);
    demarrage->setShortcut(QKeySequence(Qt::Key_S)); // START
    addAction(demarrage);
    connect(demarrage,
            SIGNAL(triggered()),
            this,
            SLOT(ouvrirPageAvantCourse()));

    QAction* demarrerCourse = new QAction(this);
    demarrerCourse->setShortcut(QKeySequence(Qt::Key_1));
    addAction(demarrerCourse);
    connect(demarrerCourse, SIGNAL(triggered()), this, SLOT(demarrerCourse()));

    QAction* selectionnerParametres = new QAction(this);
    selectionnerParametres->setShortcut(QKeySequence(Qt::Key_2));
    addAction(selectionnerParametres);
    connect(selectionnerParametres,
            SIGNAL(triggered()),
            this,
            SLOT(accederParametres())); // TODO

    QAction* retourAccueil = new QAction(this);
    retourAccueil->setShortcut(QKeySequence(Qt::Key_3));
    addAction(retourAccueil);
    connect(retourAccueil,
            SIGNAL(triggered()),
            this,
            SLOT(afficherPageConnexion()));

    QAction* quitterStatistiques = new QAction(this);
    quitterStatistiques->setShortcut(QKeySequence(Qt::Key_Q));
    addAction(quitterStatistiques);
    connect(quitterStatistiques,
            SIGNAL(triggered()),
            this,
            SLOT(quitterStatistiques()));

    QAction* afficherStatsJoueurSuivant = new QAction(this);
    afficherStatsJoueurSuivant->setShortcut(QKeySequence(Qt::Key_D));
    addAction(afficherStatsJoueurSuivant);
    connect(afficherStatsJoueurSuivant,
            SIGNAL(triggered()),
            this,
            SLOT(afficherResultatJoueurSuivant()));
}
#endif
