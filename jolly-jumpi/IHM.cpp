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
    bluetooth(new Bluetooth(this)), connecte(false), estMenu(true),
    parametreSelectionne(0), ui(new Ui::IHM),
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
    ui->setupUi(this);
    afficherWidgets();
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
    qDebug() << Q_FUNC_INFO;
    afficherPage(IHM::Page::Connexion);
}

/**
 * @fn IHM::afficherPageCourse()
 * @brief Affiche la page pour une course de chevaux
 */
void IHM::afficherPageCourse()
{
    qDebug() << Q_FUNC_INFO;
    afficherPage(IHM::Page::PageCourse);
}

void IHM::afficherPageParametres()
{
    qDebug() << Q_FUNC_INFO;
    afficherPage(IHM::Page::Parametres);
}

void IHM::afficherPageChangementParametre()
{
    qDebug() << Q_FUNC_INFO;
    afficherPage(IHM::Page::ChangementParametres);
}

void IHM::afficherPageAvantCourse()
{
    qDebug() << Q_FUNC_INFO;
    afficherPage(IHM::Page::AvantCourse);
}

void IHM::afficherPageStatistiques()
{
    qDebug() << Q_FUNC_INFO;
    optionSelectionne = 0;
    mettreEnEvidenceSelection();
    afficherPage(IHM::Page::PageStatistiques);
}

void IHM::supprimerWidgets()
{
    qDebug() << Q_FUNC_INFO;
    for(int i = 0; i < avatarsJoueurs.size(); i++)
    {
        avatarsJoueurs[i]->setPixmap(*imageAvatarsJoueurs[i]);
        ui->pages->widget(IHM::Page::PageCourse)
          ->findChild<QGridLayout*>("gridLayout")
          ->removeWidget(avatarsJoueurs[i]);
        delete avatarsJoueurs[i];
    }
    for(int i = 0; i < placeHolder.size(); i++)
    {
        placeHolder[i]->setPixmap(*imagePlaceHolder[i]);
        ui->pages->widget(IHM::Page::PageCourse)
          ->findChild<QGridLayout*>("gridLayout")
          ->removeWidget(placeHolder[i]);
        delete placeHolder[i];
    }
    imageAvatarsJoueurs.clear();
    avatarsJoueurs.clear();
    imagePlaceHolder.clear();
    placeHolder.clear();
    afficherWidgets();
}

void IHM::afficherWidgets()
{
    instancierWidgets();
    initialiserWidgets();
    positionnerWidgets();
    initialiserFenetre();
}

void IHM::instancierWidgets()
{
    for(int i = 0; i < course->getNbChevaux(); i++)
    {
        imageAvatarsJoueurs.push_back(
          new QPixmap("Images/cheval" + QString::number(i + 1) + ".png"));
        avatarsJoueurs.push_back(new QLabel(this));
        qDebug() << Q_FUNC_INFO << i + 1 << "chevaux";
    }
    for(int i = 0; i < course->getDureePartie(); i++)
    {
        imagePlaceHolder.push_back(new QPixmap("Images/placeholder.png"));
        placeHolder.push_back(new QLabel(this));
        qDebug() << Q_FUNC_INFO << i + 1 << "placeholders";
    }
}

void IHM::initialiserWidgets()
{
    qDebug() << Q_FUNC_INFO;
    QVector<unsigned int> positionCheval = course->getPositionChevaux();
    for(int i = 0; i < imageAvatarsJoueurs.size(); i++)
    {
        *imageAvatarsJoueurs[i] = imageAvatarsJoueurs[i]->scaled(
          QSize(screenGeometry.width() * 0.1, screenGeometry.height() * 0.1));
        avatarsJoueurs[i]->setPixmap(*imageAvatarsJoueurs[i]);

        ui->pages->widget(IHM::Page::PageCourse)
          ->findChild<QGridLayout*>("gridLayout")
          ->addWidget(avatarsJoueurs[i], i, positionCheval[i], Qt::AlignTop);

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

    for(int i = 0; i <= course->getDureePartie(); i++)
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
        supprimerWidgets();
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
    if(ui->pages->currentIndex() == IHM::Page::AvantCourse ||
       ui->pages->currentIndex() == IHM::Page::ChangementParametres)
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
    bluetooth->arreterReconnexion();
    ui->pages->widget(IHM::Page::Connexion)
      ->findChild<QLabel*>("etatConnexion")
      ->setText("Appuyez sur start");
    bluetooth->envoyerTrameConnexion();
}

void IHM::gererEtatDeconnexion()
{
    qDebug() << Q_FUNC_INFO;
    connecte = false;
    bluetooth->deconnecter();
}

void IHM::afficherResultatJoueurSuivant()
{
    if(ui->pages->currentIndex() == IHM::Page::PageStatistiques &&
       course->getNbChevaux() != 1)
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

        if(positionChevaux[i] == course->getDureePartie())
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
    float record          = stats->getRecord();
    ui->pages->widget(IHM::Page::PageStatistiques)
      ->findChild<QLabel*>("duree")
      ->setText(
        QString::number(dureeDeLaPartie, 'f', 2) +
        " secondes (Meilleur score: " + QString::number(record, 'f', 2) + "s)");
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
        if(pointsParTir == 1)
            ui->pages->widget(IHM::Page::PageStatistiques)
            ->findChild<QLabel*>("ppt")
            ->setText(QString::number(pointsParTir) + " point par tir");
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
      (positionChevaux[numeroJoueur] * 100) / course->getDureePartie();
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

    if(ui->pages->currentIndex() == IHM::Page::Parametres)
    {
        if(optionSelectionne > (MenuParametres::NbOptionsParametres - 1))
        {
            optionSelectionne = 0;
        }
        if(optionSelectionne < 0)
        {
            optionSelectionne = (MenuParametres::NbOptionsParametres - 1);
        }
    }

    if(ui->pages->currentIndex() == IHM::Page::ChangementParametres)
    {
        qDebug() << Q_FUNC_INFO << "currentIndex"
                 << "ChangementParametres";
        switch(parametreSelectionne)
        {
            case ChangerParametres::NombreJoueurs:
                if(optionSelectionne > course->getNbChevauxMax())
                {
                    optionSelectionne = 1;
                }
                if(optionSelectionne < 1)
                {
                    optionSelectionne = course->getNbChevauxMax();
                }
                break;

            case ChangerParametres::Distance:
                if(optionSelectionne > course->getDureeMax())
                {
                    optionSelectionne = course->getDureeMin();
                }
                if(optionSelectionne < course->getDureeMin())
                {
                    optionSelectionne = course->getDureeMax();
                }
                break;

            case ChangerParametres::ModeDeJeu:
                if(optionSelectionne >= ModeDeJeu::NbModes)
                {
                    optionSelectionne = 0;
                }
                if(optionSelectionne < 0)
                {
                    optionSelectionne = (ModeDeJeu::NbModes - 1);
                }
                break;
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
                  ->findChild<QLabel*>("b_quitter")
                  ->setFont(policeStats);
                break;
            case MenuStatistiques::JoueurSuivant:
                ui->pages->widget(IHM::Page::PageStatistiques)
                  ->findChild<QLabel*>("a_joueurSuivant")
                  ->setFont(policeStats);
                break;
            default:
                break;
        };
    }

    if(ui->pages->currentIndex() == IHM::Page::Parametres)
    {
        qDebug() << Q_FUNC_INFO << "currentIndex"
                 << "Parametres";
        switch(optionSelectionne)
        {
            case MenuParametres::ChangerNombreJoueurs:
                ui->pages->widget(IHM::Page::Parametres)
                  ->findChild<QLabel*>("nombreJoueurs")
                  ->setFont(policeSelectionne);
                break;
            case MenuParametres::ChangerDureePartie:
                ui->pages->widget(IHM::Page::Parametres)
                  ->findChild<QLabel*>("dureePartie")
                  ->setFont(policeSelectionne);
                break;
            case MenuParametres::ChangerModeDeJeu:
                ui->pages->widget(IHM::Page::Parametres)
                  ->findChild<QLabel*>("modeJeu")
                  ->setFont(policeSelectionne);
                break;
            case MenuParametres::QuitterParametres:
                ui->pages->widget(IHM::Page::Parametres)
                  ->findChild<QLabel*>("quitter")
                  ->setFont(policeSelectionne);
                break;
            default:
                break;
        };
    }

    if(ui->pages->currentIndex() == IHM::Page::ChangementParametres)
    {
        if(parametreSelectionne == ChangerParametres::Distance)
        {
            ui->pages->widget(IHM::Page::ChangementParametres)
              ->findChild<QLabel*>("selection")
              ->setText(QString::number(optionSelectionne) + " points");
            course->setDureePartie(optionSelectionne);
            qDebug() << Q_FUNC_INFO << "dureePartie"
                     << course->getDureePartie();
        }
        if(parametreSelectionne == ChangerParametres::NombreJoueurs)
        {
            if(optionSelectionne != 1)
            {
                ui->pages->widget(IHM::Page::ChangementParametres)
                  ->findChild<QLabel*>("selection")
                  ->setText(QString::number(optionSelectionne) + " joueurs");
            }
            else
            {
                ui->pages->widget(IHM::Page::ChangementParametres)
                  ->findChild<QLabel*>("selection")
                  ->setText(QString::number(optionSelectionne) + " joueur");
            }

            course->setNbChevaux(optionSelectionne);
            qDebug() << Q_FUNC_INFO << "nbChevaux" << course->getNbChevaux();
        }
        if(parametreSelectionne == ChangerParametres::ModeDeJeu)
        {
            if(optionSelectionne == 0)
            {
                ui->pages->widget(IHM::Page::ChangementParametres)
                  ->findChild<QLabel*>("selection")
                  ->setText("Classique");
            }
            if(optionSelectionne == ModeDeJeu::PointsAleatoire)
            {
                ui->pages->widget(IHM::Page::ChangementParametres)
                  ->findChild<QLabel*>("selection")
                  ->setText("Points aléatoire");
            }

            course->setModeDeJeu(optionSelectionne);
        }
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
      ->findChild<QLabel*>("b_quitter")
      ->setFont(police);

    ui->pages->widget(IHM::Page::PageStatistiques)
      ->findChild<QLabel*>("a_joueurSuivant")
      ->setFont(police);

    ui->pages->widget(IHM::Page::Parametres)
      ->findChild<QLabel*>("dureePartie")
      ->setFont(police);

    ui->pages->widget(IHM::Page::Parametres)
      ->findChild<QLabel*>("nombreJoueurs")
      ->setFont(police);

    ui->pages->widget(IHM::Page::Parametres)
      ->findChild<QLabel*>("modeJeu")
      ->setFont(police);

    ui->pages->widget(IHM::Page::Parametres)
      ->findChild<QLabel*>("quitter")
      ->setFont(police);
}

void IHM::validerSelection()
{
    qDebug() << Q_FUNC_INFO << "optionSelectionne" << optionSelectionne;
    if(!estMenu)
    {
        estMenu = true;
        afficherPageParametres();
        qDebug() << Q_FUNC_INFO << "nbChevaux" << course->getNbChevaux()
                 << "dureePartie" << course->getDureePartie();
        mettreEnEvidenceSelection();
        return;
    }
    if((ui->pages->currentIndex() == IHM::Page::Parametres) && estMenu)
    {
        qDebug() << Q_FUNC_INFO << "currentIndex"
                 << "Parametres"
                 << "estMenu" << estMenu;
        parametreSelectionne = optionSelectionne;
        switch(optionSelectionne)
        {
            case MenuParametres::ChangerNombreJoueurs:
                estMenu = false;
                changerNombreJoueurs();
                return;
                break;
            case MenuParametres::ChangerDureePartie:
                estMenu = false;
                changerDureePartie();
                return;
                break;
            case MenuParametres::ChangerModeDeJeu:
                estMenu = false;
                changerModeDeJeu();
                return;
                break;
            case MenuParametres::QuitterParametres:
                estMenu = true;
                afficherPageAvantCourse();
                optionSelectionne = 0;
                mettreEnEvidenceSelection();
                return;
                break;
            default:
                return;
                break;
        };
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
                return;
                break;
            default:
                break;
        };
    }
    if(ui->pages->currentIndex() == IHM::Page::Connexion)
    {
        ouvrirPageAvantCourse();
    }
    if(ui->pages->currentIndex() == IHM::Page::PageStatistiques)
    {
        switch(optionSelectionne)
        {
            case MenuStatistiques::QuitterStatistiques:
                optionSelectionne = 0;
                mettreEnEvidenceSelection();
                quitterStatistiques();
                break;
            case MenuStatistiques::JoueurSuivant:
                afficherResultatJoueurSuivant();
                break;
            default:
                break;
        };
    }
    qDebug() << Q_FUNC_INFO;
}

void IHM::changerNombreJoueurs()
{
    afficherPageChangementParametre();
    optionSelectionne = course->getNbChevaux();
    qDebug() << Q_FUNC_INFO << "optionSelectionne" << optionSelectionne;
    ui->pages->widget(IHM::Page::ChangementParametres)
      ->findChild<QLabel*>("parametre")
      ->setText("Nombre de joueurs");
    ui->pages->widget(IHM::Page::ChangementParametres)
      ->findChild<QLabel*>("selection")
      ->setText(QString::number(optionSelectionne) + " joueurs");
}

void IHM::changerDureePartie()
{
    afficherPageChangementParametre();
    optionSelectionne = course->getDureeMax();

    ui->pages->widget(IHM::Page::ChangementParametres)
      ->findChild<QLabel*>("parametre")
      ->setText("Durée de la partie");
    course->setDureePartie(optionSelectionne);

    ui->pages->widget(IHM::Page::ChangementParametres)
      ->findChild<QLabel*>("selection")
      ->setText(QString::number(optionSelectionne) + " points");
}

void IHM::changerModeDeJeu()
{
    afficherPageChangementParametre();
    course->setModeDeJeu(optionSelectionne);

    ui->pages->widget(IHM::Page::ChangementParametres)
      ->findChild<QLabel*>("parametre")
      ->setText("Mode de jeu");

    ui->pages->widget(IHM::Page::ChangementParametres)
      ->findChild<QLabel*>("selection")
      ->setText("Classique");
}
