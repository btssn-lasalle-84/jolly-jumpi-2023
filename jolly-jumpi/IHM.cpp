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

IHM::IHM(QWidget *parent)
    : QWidget(parent), ui(new Ui::IHM), positionChevaux(NB_CHEVAUX_MAX, 0),
      nbChevaux(positionChevaux.size()) {
  qDebug() << Q_FUNC_INFO << "nbChevaux" << nbChevaux;

  instancierWidgets();
  initialiserWidgets();
  positionnerWidgets();
  connecterSignauxSlots();
  initialiserFenetre();
#ifdef MODE_SIMULATION
  installerModeSimulation();
#endif
  afficherPageCourse();
}

/**
 * @brief Destructeur de la classe IHM
 *
 * @fn IHM::~IHM
 * @details Libère les ressources de l'application
 */

IHM::~IHM() { delete ui; }

/**
 * @fn IHM::afficherPage(IHM::Page page)
 * @brief Selectionne et affiche la page passée en paramètre
 */
void IHM::afficherPage(IHM::Page page) { ui->pages->setCurrentIndex(page); }

/**
 * @fn IHM::afficherPageConnexion()
 * @brief Affiche la page de connexion
 */
void IHM::afficherPageConnexion() { afficherPage(IHM::Page::Connexion); }

/**
 * @fn IHM::afficherPageCourse()
 * @brief Affiche la page pour une course de chevaux
 */
void IHM::afficherPageCourse() { afficherPage(IHM::Page::Course); }

void IHM::instancierWidgets() {
  ui->setupUi(this);

  for (int i = 0; i < NB_CHEVAUX_MAX; i++) {
    imageAvatarsJoueurs.push_back(
        new QPixmap("Images/cheval" + QString::number(i + 1) + ".png"));
    avatarsJoueurs.push_back(new QLabel(this));
  }

  imagePlaceHolder = new QPixmap("Images/placeholder.png");
  placeHolder = new QLabel(this);
}

void IHM::initialiserWidgets() {
  for (int i = 0; i < imageAvatarsJoueurs.size(); i++) {
    *imageAvatarsJoueurs[i] = imageAvatarsJoueurs[i]->scaled(
        QSize(qApp->desktop()->availableGeometry(this).width() * 0.1,
              qApp->desktop()->availableGeometry(this).height() * 0.1));
    avatarsJoueurs[i]->setPixmap(*imageAvatarsJoueurs[i]);

    ui->pages->widget(IHM::Page::Course)
        ->findChild<QGridLayout *>("gridLayout")
        ->addWidget(avatarsJoueurs[i], i, 0, Qt::AlignTop);

    ui->pages->widget(IHM::Page::Course)
        ->findChild<QGridLayout *>("gridLayout")
        ->setRowStretch(imageAvatarsJoueurs.size(), 1);
  }
  *imagePlaceHolder = imagePlaceHolder->scaled(
      QSize(qApp->desktop()->availableGeometry(this).width() * 0.1,
            qApp->desktop()->availableGeometry(this).height() * 0.1));
  placeHolder->setPixmap(*imagePlaceHolder);

  for (int i = 0; i < DISTANCE_MAX; i++) {
    *imagePlaceHolder = imagePlaceHolder->scaled(
        QSize(qApp->desktop()->availableGeometry(this).width() * 0.1,
              qApp->desktop()->availableGeometry(this).height() * 0.1));
    placeHolder->setPixmap(*imagePlaceHolder);
    ui->pages->widget(IHM::Page::Course)
        ->findChild<QGridLayout *>("gridLayout")
        ->addWidget(placeHolder, nbChevaux, i, Qt::AlignTop);
    ui->pages->widget(IHM::Page::Course)
        ->findChild<QGridLayout *>("gridLayout")
        ->setRowStretch(imageAvatarsJoueurs.size(), 1);
  }

  ui->pages->widget(IHM::Page::Course)
      ->findChild<QGridLayout *>("gridLayout")
      ->setRowStretch(imageAvatarsJoueurs.size(), 1);

  for (int i = 0; i < DISTANCE_MAX; i++) {
    ui->pages->widget(IHM::Page::Course)
        ->findChild<QGridLayout *>("gridLayout")
        ->setColumnStretch(i, 1);
  }
}

void IHM::positionnerWidgets() {
  ui->pages->widget(IHM::Page::Course)
      ->findChild<QGridLayout *>("gridLayout")
      ->setContentsMargins(
          0, qApp->desktop()->availableGeometry(this).height() * 0.14, 0, 0);
}

void IHM::connecterSignauxSlots() {}

void IHM::initialiserFenetre() {
#ifdef RASPBERRY_PI
  showFullScreen();
#else
  setFixedSize(qApp->desktop()->availableGeometry(this).width(),
               qApp->desktop()->availableGeometry(this).height());
  // showMaximized();
#endif

  afficherPageConnexion();
}

bool IHM::estPartieFinie() {
  for (int i = 0; i < nbChevaux; i++) {
    if (positionChevaux[i] == DISTANCE_MAX) {
      qDebug() << Q_FUNC_INFO << "true";
      return true;
    }
  }
  qDebug() << Q_FUNC_INFO << "false";
  return false;
}

#ifdef MODE_SIMULATION
void IHM::installerModeSimulation() {
  QAction *actionAvancementCheval = new QAction(this);
  actionAvancementCheval->setShortcut(QKeySequence(Qt::Key_Right));
  addAction(actionAvancementCheval);
  connect(actionAvancementCheval, SIGNAL(triggered()), this,
          SLOT(simulerAvancementCheval()));
}

int IHM::randInt(int min, int max) { return qrand() % ((max + 1) - min) + min; }
#endif

void IHM::actualiserPositionChevaux(int numeroCheval, Trou deplacement) {
  qDebug() << Q_FUNC_INFO << "numeroCheval" << numeroCheval << "deplacement"
           << int(deplacement);
  positionChevaux[numeroCheval] =
      positionChevaux[numeroCheval] + int(deplacement);
  if (positionChevaux[numeroCheval] > DISTANCE_MAX)
    positionChevaux[numeroCheval] = DISTANCE_MAX;
  avancerChevaux();
}

void IHM::avancerChevaux() {
  for (int i = 0; i < nbChevaux; i++) {
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
  }
}

#ifdef MODE_SIMULATION
void IHM::simulerAvancementCheval() {
  Trou trous[NB_COULEURS_TROU] = {JAUNE, BLEU, ROUGE};
  int numeroCheval = randInt(0, NB_CHEVAUX_MAX - 1);
  int trou = randInt(0, NB_COULEURS_TROU - 1);
  actualiserPositionChevaux(numeroCheval, trous[trou]);
}
#endif
