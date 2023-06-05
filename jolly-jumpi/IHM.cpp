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
    QWidget(parent), ui(new Ui::IHM), nbChevaux(NB_CHEVAUX_MAX),
    positionChevaux(nbChevaux, 0), classement(nbChevaux, 0),
    joueurGagnant(AUCUN_JOUEUR), positionClassement(0),
    screen(QGuiApplication::primaryScreen()),
    screenGeometry(screen->availableGeometry().size()), chronometre(0.0),
    dureeDeLaPartie(0.0), nombreTirs(nbChevaux, 0), nombrePoints(nbChevaux, 0),
    course(false)
{
    qDebug() << Q_FUNC_INFO << "nbChevaux" << nbChevaux;

    instancierWidgets();
    initialiserWidgets();
    positionnerWidgets();
    initialiserChronometre();
    connecterSignauxSlots();
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
    qDebug() << Q_FUNC_INFO;
    ui->pages->widget(IHM::Page::Course)
      ->findChild<QGridLayout*>("gridLayout")
      ->setContentsMargins(0, screenGeometry.height() * 0.14, 0, 0);
}

void IHM::initialiserChronometre()
{
    timer = new QTimer(this);
    timer->setInterval(10);
}

void IHM::connecterSignauxSlots()
{
    connect(timer, SIGNAL(timeout()), this, SLOT(chronometrer()));
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

void IHM::initialiserCourse()
{
    joueurGagnant      = AUCUN_JOUEUR;
    positionClassement = 0;
    for(int i = 0; i < nbChevaux; i++)
    {
        positionChevaux[i] = 0;
    }
    timer->start();
    avancerChevaux();
    course = true;
}

bool IHM::estCourseFinie()
{
    for(int i = 0; i < nbChevaux; i++)
    {
        if(positionChevaux[i] == DISTANCE_MAX)
        {
            joueurGagnant = i;
            qDebug() << Q_FUNC_INFO << "DISTANCE_MAX"
                     << "true"
                     << "joueurGagnant" << joueurGagnant;
            return true;
        }
    }
    qDebug() << Q_FUNC_INFO << "DISTANCE_MAX"
             << "false";
    return false;
}

void IHM::terminerCourse()
{
    course          = false;
    dureeDeLaPartie = chronometre;
    determinerClassement();
    afficherResultats();
    timer->stop();
    QTimer::singleShot(ATTENTE_FIN_COURSE, this, SLOT(attendreFinCourse()));
}

void IHM::determinerClassement()
{
    if(joueurGagnant == AUCUN_JOUEUR)
        return;
    qDebug() << Q_FUNC_INFO << "positionChevaux" << positionChevaux;
    /**
     * @see QMap
     */
    classement.clear();
    classement = positionChevaux;
    std::sort(classement.begin(),
              classement.end(),
              std::greater<unsigned int>());
    positionClassement = 1;
    qDebug() << Q_FUNC_INFO << "classement" << classement;
    qDebug() << Q_FUNC_INFO << "joueurGagnant" << joueurGagnant;
}

int IHM::determinerJoueurSuivant()
{
    if(positionClassement > nbChevaux)
    {
        qDebug() << Q_FUNC_INFO;
        positionClassement = 1;
        return joueurGagnant;
    }
    /**
     * @fixme Il y a un risque de confusion avec les joueurs à égalité. Ne
     * faudrait-il pas effacer la valeur de positionChevaux[numeroJoueur]
     * lorsqu'il a été déjà classé ? Avec DISTANCE_MAX+1 par exemple ?
     */
    for(int numeroJoueur = 0; numeroJoueur < classement.size(); numeroJoueur++)
    {
        if(classement[positionClassement - 1] == positionChevaux[numeroJoueur])
        {
            qDebug() << Q_FUNC_INFO << "numeroJoueur" << numeroJoueur;
            return numeroJoueur;
        }
    }
    return AUCUN_JOUEUR;
}

void IHM::afficherResultats()
{
    qDebug() << Q_FUNC_INFO;
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
    {
        int joueurSuivant = determinerJoueurSuivant();
        qDebug() << Q_FUNC_INFO << "positionClassement" << positionClassement
                 << "joueurSuivant" << joueurSuivant;
        afficherClassement(positionClassement);
        afficherPositionFinale(joueurSuivant);
        afficherNumeroJoueur(joueurSuivant);
        afficherPointsParSeconde(joueurSuivant);
        afficherNombrePointsParTir(joueurSuivant);
        positionClassement++;
    }
}

void IHM::afficherDureePartie()
{
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

void IHM::afficherPositionFinale(int numeroJoueur)
{
    if(numeroJoueur == AUCUN_JOUEUR)
        return;
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

void IHM::afficherNombrePointsParTir(int numeroJoueur)
{
    if(numeroJoueur == AUCUN_JOUEUR)
        return;
    unsigned int pointsParTir = 0;

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

void IHM::afficherNumeroJoueur(int numeroJoueur)
{
    if(numeroJoueur == AUCUN_JOUEUR)
        return;
    if(numeroJoueur == joueurGagnant)
        ui->pages->widget(IHM::Page::Statistiques)
          ->findChild<QLabel*>("position")
          ->setText("Gagnant : joueur " + QString::number(joueurGagnant + 1));
    else
    {
        ui->pages->widget(IHM::Page::Statistiques)
          ->findChild<QLabel*>("position")
          ->setText("Joueur " + QString::number(numeroJoueur + 1));
    }
}

void IHM::attendreFinCourse()
{
    chronometre = 0;
    afficherPageStatistiques();
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

void IHM::demarrerCourse()
{
    if(ui->pages->currentIndex() == IHM::Page::AvantCourse)
    {
        initialiserCourse();
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
    if(ui->pages->currentIndex() == IHM::Page::AvantCourse)
    {
        afficherPageConnexion();
    }
}

void IHM::quitterStatistiques()
{
    qDebug() << Q_FUNC_INFO;
    if(ui->pages->currentIndex() == IHM::Page::Statistiques)
    {
        afficherPageAvantCourse();
    }
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

        if(positionChevaux[i] == DISTANCE_MAX)
            classement.push_back(i);
    }
    if(estCourseFinie())
        terminerCourse();
}

int IHM::randInt(int min, int max)
{
    return ((QRandomGenerator::global()->bounded(min, max + 1)) + min);
}
#endif

void IHM::chronometrer()
{
    chronometre += 0.01;
}

void IHM::actualiserPositionChevaux(int numeroCheval, Trou deplacement)
{
    if(!course)
        return;
    ++nombreTirs[numeroCheval];
    nombrePoints[numeroCheval] += deplacement;
    if(nombrePoints[numeroCheval] >= DISTANCE_MAX)
        nombrePoints[numeroCheval] = DISTANCE_MAX;
    qDebug() << Q_FUNC_INFO << "Le cheval numéro" << numeroCheval + 1
             << ", qui était positionné à la case"
             << positionChevaux[numeroCheval] << "a avancé de"
             << int(deplacement);
    qDebug() << Q_FUNC_INFO << "chronometre" << chronometre;

    qDebug() << Q_FUNC_INFO << "nombreTirs" << nombreTirs[numeroCheval]
             << "nombrePoints" << nombrePoints[numeroCheval];

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

#ifdef MODE_SIMULATION
void IHM::simulerAvancementCheval()
{
    if(ui->pages->currentIndex() == IHM::Page::Course && course)
    {
        Trou trous[NB_COULEURS_TROU] = { JAUNE, BLEU, ROUGE };
        int  numeroCheval            = randInt(0, NB_CHEVAUX_MAX - 1);
        int  trou                    = randInt(0, NB_COULEURS_TROU - 1);
        actualiserPositionChevaux(numeroCheval, trous[trou]);
    }
}
#endif
