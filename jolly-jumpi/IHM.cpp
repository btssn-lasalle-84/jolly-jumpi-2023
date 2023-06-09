/**
 * @file IHM.cpp
 *
 * @brief Définition de la classe IHM
 * @author Axel LAPIERRE
 * @version 0.1
 */

#include "IHM.h"
#include "ui_IHM.h"
#include "course.h"
#include "statistiques.h"

/**
 * @brief Constructeur de la classe IHM
 *
 * @fn IHM::IHM
 * @param parent nullptr pour définir la fenêtre principale de l'application
 */

IHM::IHM(QWidget* parent) :
    QWidget(parent), course(new Course(this)), stats(new Statistiques(this)),
    bluetooth(new Bluetooth(this)), connecte(false), ui(new Ui::IHM),
    screen(QGuiApplication::primaryScreen()),
    screenGeometry(screen->availableGeometry().size()), optionSelectionne(0),
    police("Ubuntu Regular", 40, QFont::AnyStyle),
    policeStats("Ubuntu Regular", 60, QFont::Bold),
    policeSelectionne("Ubuntu Condensed", 60, QFont::Bold)
{
    qDebug() << Q_FUNC_INFO;
    course->setStatistiques(stats);
    course->setBluetooth(bluetooth);
    bluetooth->setCourse(course);
    stats->setCourse(course);

    bluetooth->initialiserCommunication();
    // bluetooth->connecter(ADRESSE_ESP32_SIMULATEUR);
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
    qDebug() << Q_FUNC_INFO;
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
    afficherPage(IHM::Page::PageCourse);
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
    afficherPage(IHM::Page::PageStatistiques);
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

        ui->pages->widget(IHM::Page::PageCourse)
          ->findChild<QGridLayout*>("gridLayout")
          ->addWidget(avatarsJoueurs[i], i, 0, Qt::AlignTop);

        ui->pages->widget(IHM::Page::PageCourse)
          ->findChild<QGridLayout*>("gridLayout")
          ->setRowStretch(imageAvatarsJoueurs.size(), 1);

        *imagePlaceHolder[i] = imagePlaceHolder[i]->scaled(
          QSize(screenGeometry.width() * 0.1, screenGeometry.height() * 0.1));
        placeHolder[i]->setPixmap(*imagePlaceHolder[i]);

        ui->pages->widget(IHM::Page::PageCourse)
          ->findChild<QGridLayout*>("gridLayout")
          ->addWidget(placeHolder[i], course->getNbChevaux(), i, Qt::AlignTop);

        ui->pages->widget(IHM::Page::PageCourse)
          ->findChild<QGridLayout*>("gridLayout")
          ->setRowStretch(imagePlaceHolder.size(), 1);
    }

    ui->pages->widget(IHM::Page::PageCourse)
      ->findChild<QGridLayout*>("gridLayout")
      ->setRowStretch(imageAvatarsJoueurs.size(), 1);

    for(int i = 0; i <= DISTANCE_MAX; i++)
    {
        ui->pages->widget(IHM::Page::PageCourse)
          ->findChild<QGridLayout*>("gridLayout")
          ->setColumnStretch(i, 1);
    }
}

void IHM::positionnerWidgets()
{
    qDebug() << Q_FUNC_INFO;
    ui->pages->widget(IHM::Page::PageCourse)
      ->findChild<QGridLayout*>("gridLayout")
      ->setContentsMargins(0, screenGeometry.height() * 0.14, 0, 0);
}

void IHM::initialiserFenetre()
{
#ifdef RASPBERRY_PI
    setFixedSize(screenGeometry.width(), screenGeometry.height());
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
    qDebug() << Q_FUNC_INFO;
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
    qDebug() << Q_FUNC_INFO;
    if(ui->pages->currentIndex() == IHM::Page::AvantCourse)
    {
        afficherPageConnexion();
    }
}

void IHM::quitterStatistiques()
{
    qDebug() << Q_FUNC_INFO;
    if(ui->pages->currentIndex() == IHM::Page::PageStatistiques)
    {
        afficherPageAvantCourse();
    }
}

void IHM::gererEtatConnexion()
{
    qDebug() << Q_FUNC_INFO;
    connecte = true;
    bluetooth->envoyerTrameConnexion();
}

void IHM::gererEtatDeconnexion()
{
    qDebug() << Q_FUNC_INFO;
    connecte = false;
}

void IHM::afficherResultatJoueurSuivant()
{
    if(ui->pages->currentIndex() == IHM::Page::PageStatistiques)
        stats->afficherResultatJoueurSuivant();
}

void IHM::avancerChevaux()
{
    QVector<unsigned int> positionChevaux = course->getPositionChevaux();
    QVector<unsigned int> classement      = stats->getClassement();
    for(int i = 0; i < course->getNbChevaux(); i++)
    {
        avatarsJoueurs[i]->setPixmap(*imageAvatarsJoueurs[i]);
        ui->pages->widget(IHM::Page::PageCourse)
          ->findChild<QGridLayout*>("gridLayout")
          ->removeWidget(avatarsJoueurs[i]);

        ui->pages->widget(IHM::Page::PageCourse)
          ->findChild<QGridLayout*>("gridLayout")
          ->addWidget(avatarsJoueurs[i], i, positionChevaux[i], Qt::AlignTop);

        ui->pages->widget(IHM::Page::PageCourse)
          ->findChild<QGridLayout*>("gridLayout")
          ->setRowStretch(imageAvatarsJoueurs.size(), 1);

        if(positionChevaux[i] == DISTANCE_MAX)
            classement.push_back(i);
    }
}

bool IHM::estBonIndex()
{
    if(ui->pages->currentIndex() == IHM::PageCourse && course)
        return true;
    return false;
}

void IHM::afficherDureePartie()
{
    float dureeDeLaPartie = stats->getDureeDeLaPartie();
    ui->pages->widget(IHM::Page::PageStatistiques)
      ->findChild<QLabel*>("duree")
      ->setText(QString::number(dureeDeLaPartie, 'f', 2) + " secondes");
    qDebug() << Q_FUNC_INFO << "dureeDeLaPartie" << dureeDeLaPartie;
}

void IHM::afficherClassement(int positionClassement)
{
    ui->pages->widget(IHM::Page::PageStatistiques)
      ->findChild<QLabel*>("duree")
      ->setText(QString::number(positionClassement) + "e position");
}

void IHM::afficherNombrePointsParTir(int numeroJoueur)
{
    if(numeroJoueur == AUCUN_JOUEUR)
        return;
    unsigned int          pointsParTir    = 0;
    QVector<unsigned int> positionChevaux = course->getPositionChevaux();
    QVector<unsigned int> nombrePoints    = stats->getNombrePoints();
    QVector<unsigned int> nombreTirs      = stats->getNombreTirs();
    if(positionChevaux[numeroJoueur] != 0)
    {
        qDebug() << Q_FUNC_INFO << "numeroJoueur" << numeroJoueur
                 << "nombrePoints" << nombrePoints[numeroJoueur] << "nombreTirs"
                 << nombreTirs[numeroJoueur];
        pointsParTir = (nombrePoints[numeroJoueur] / nombreTirs[numeroJoueur]);
        qDebug() << Q_FUNC_INFO << "numeroJoueur" << numeroJoueur
                 << "pointsParTir" << pointsParTir;
        ui->pages->widget(IHM::Page::PageStatistiques)
          ->findChild<QLabel*>("ppt")
          ->setText(QString::number(pointsParTir) + " points par tir");
    }
    else
    {
        ui->pages->widget(IHM::Page::PageStatistiques)
          ->findChild<QLabel*>("ppt")
          ->setText("0 point par tir");
    }
}

void IHM::afficherPositionFinale(int numeroJoueur)
{
    if(numeroJoueur == AUCUN_JOUEUR)
        return;
    QVector<unsigned int> positionChevaux = course->getPositionChevaux();
    unsigned int          pourcentageCompletion =
      (positionChevaux[numeroJoueur] * 100) / DISTANCE_MAX;
    qDebug() << Q_FUNC_INFO << "numeroJoueur" << numeroJoueur
             << "pourcentageCompletion" << pourcentageCompletion;
    ui->pages->widget(IHM::Page::PageStatistiques)
      ->findChild<QLabel*>("temps")
      ->setText("Course finie à " + QString::number(pourcentageCompletion) +
                "%");
}

void IHM::afficherPointsParSeconde(int numeroJoueur)
{
    if(numeroJoueur == AUCUN_JOUEUR)
        return;
    float                 pointsParSeconde = 0.0;
    QVector<unsigned int> positionChevaux  = course->getPositionChevaux();
    QVector<unsigned int> nombrePoints     = stats->getNombrePoints();
    float                 dureeDeLaPartie  = stats->getDureeDeLaPartie();
    if(positionChevaux[numeroJoueur] != 0)
    {
        qDebug() << Q_FUNC_INFO << "positionChevaux[numeroJoueur]"
                 << positionChevaux[numeroJoueur];
        pointsParSeconde = (nombrePoints[numeroJoueur] / dureeDeLaPartie);
        qDebug() << Q_FUNC_INFO << "numeroJoueur" << numeroJoueur
                 << "pointsParSeconde" << pointsParSeconde;
        ui->pages->widget(IHM::Page::PageStatistiques)
          ->findChild<QLabel*>("pps")
          ->setText(QString::number(pointsParSeconde, 'f', 2) +
                    " points par seconde");
    }
    else
    {
        ui->pages->widget(IHM::Page::PageStatistiques)
          ->findChild<QLabel*>("pps")
          ->setText("0 point par seconde");
    }
}

void IHM::afficherNumeroJoueur(int numeroJoueur)
{
    if(numeroJoueur == AUCUN_JOUEUR)
        return;
    if(numeroJoueur == stats->getJoueurGagnant())
        ui->pages->widget(IHM::Page::PageStatistiques)
          ->findChild<QLabel*>("position")
          ->setText("Gagnant : joueur " +
                    QString::number(stats->getJoueurGagnant() + 1));
    else
    {
        ui->pages->widget(IHM::Page::PageStatistiques)
          ->findChild<QLabel*>("position")
          ->setText("Joueur " + QString::number(numeroJoueur + 1));
    }
}

bool IHM::estConnecte()
{
    return connecte;
}

#ifdef MODE_SIMULATION
void IHM::simulerAvancementCheval()
{
    course->simulerAvancementCheval();
}

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
            SLOT(accederParametres()));

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

void IHM::selectionnerSuivant()
{
    optionSelectionne += 1;
    qDebug() << Q_FUNC_INFO << "optionSelectionne" << optionSelectionne;
    changerSelection();
}

void IHM::selectionnerPrecedent()
{
    optionSelectionne -= 1;
    qDebug() << Q_FUNC_INFO << "optionSelectionne" << optionSelectionne;
    changerSelection();
}

void IHM::changerSelection()
{
    qDebug() << Q_FUNC_INFO << "optionSelectionne" << optionSelectionne;
    if(ui->pages->currentIndex() == IHM::Page::AvantCourse)
    {
        if(optionSelectionne > (MenuAvantCourse::NombreOptions - 1))
        {
            optionSelectionne = 0;
        }
        if(optionSelectionne < 0)
        {
            optionSelectionne = (MenuAvantCourse::NombreOptions - 1);
        }
    }

    if(ui->pages->currentIndex() == IHM::Page::PageStatistiques)
    {
        if(optionSelectionne > (MenuStatistiques::NbOptions - 1))
        {
            optionSelectionne = 0;
        }
        if(optionSelectionne < 0)
        {
            optionSelectionne = (MenuStatistiques::NbOptions - 1);
        }
    }
    mettreEnEvidenceSelection();
}

void IHM::mettreEnEvidenceSelection()
{
    deselectionner();

    qDebug() << Q_FUNC_INFO << "optionSelectionne" << optionSelectionne;
    if(ui->pages->currentIndex() == IHM::Page::AvantCourse)
    {
        qDebug() << Q_FUNC_INFO << "currentIndex"
                 << "AvantCourse";
        switch(optionSelectionne)
        {
            case MenuAvantCourse::LancerPartie:
                ui->pages->widget(IHM::Page::AvantCourse)
                  ->findChild<QLabel*>("option1")
                  ->setFont(policeSelectionne);
                break;
            case MenuAvantCourse::AccederParametres:
                ui->pages->widget(IHM::Page::AvantCourse)
                  ->findChild<QLabel*>("option2")
                  ->setFont(policeSelectionne);
                break;
            case MenuAvantCourse::Quitter:
                ui->pages->widget(IHM::Page::AvantCourse)
                  ->findChild<QLabel*>("option3")
                  ->setFont(policeSelectionne);
                break;
            default:
                break;
        };
    }

    if(ui->pages->currentIndex() == IHM::Page::PageStatistiques)
    {
        qDebug() << Q_FUNC_INFO << "currentIndex"
                 << "Statistiques";
        switch(optionSelectionne)
        {
            case MenuStatistiques::QuitterStatistiques:

                ui->pages->widget(IHM::Page::PageStatistiques)
                  ->findChild<QLabel*>("a_quitter")
                  ->setFont(policeStats);

                break;
            case MenuStatistiques::JoueurSuivant:
                ui->pages->widget(IHM::Page::PageStatistiques)
                  ->findChild<QLabel*>("b_joueurSuivant")
                  ->setFont(policeStats);
                break;
            default:
                break;
        };
    }
}

void IHM::deselectionner()
{
    qDebug() << Q_FUNC_INFO;
    ui->pages->widget(IHM::Page::AvantCourse)
      ->findChild<QLabel*>("option1")
      ->setFont(police);
    ui->pages->widget(IHM::Page::AvantCourse)
      ->findChild<QLabel*>("option2")
      ->setFont(police);
    ui->pages->widget(IHM::Page::AvantCourse)
      ->findChild<QLabel*>("option3")
      ->setFont(police);

    ui->pages->widget(IHM::Page::PageStatistiques)
      ->findChild<QLabel*>("a_quitter")
      ->setFont(police);

    ui->pages->widget(IHM::Page::PageStatistiques)
      ->findChild<QLabel*>("b_joueurSuivant")
      ->setFont(police);
}

void IHM::validerSelection()
{
    qDebug() << Q_FUNC_INFO << "optionSelectionne" << optionSelectionne;
    if(ui->pages->currentIndex() == IHM::Page::Connexion)
    {
        ouvrirPageAvantCourse();
        return;
    }
    if(ui->pages->currentIndex() == IHM::Page::AvantCourse)
    {
        switch(optionSelectionne)
        {
            case MenuAvantCourse::LancerPartie:
                demarrerCourse();
                break;
            case MenuAvantCourse::AccederParametres:
                accederParametres();
                break;
            case MenuAvantCourse::Quitter:
                quitterProgramme();
                break;
            default:
                break;
        };
    }
    if(ui->pages->currentIndex() == IHM::Page::PageStatistiques)
    {
        switch(optionSelectionne)
        {
            case MenuStatistiques::QuitterStatistiques:
                quitterStatistiques();
                break;
            case MenuStatistiques::JoueurSuivant:
                afficherResultatJoueurSuivant();
                break;
            default:
                break;
        };
    }
}
