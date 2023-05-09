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
    : QWidget(parent), ui(new Ui::IHM), positionChevaux({0, 0, 0, 0, 0, 0}) {
  qDebug() << Q_FUNC_INFO;

  instancierWidgets();
  initialiserWidgets();
  positionnerWidgets();
  connecterSignauxSlots();
  initialiserFenetre();

  afficherPageCourse();

  while (!partieFinie(positionChevaux)) {
    actualiserPositionChevaux(positionChevaux);
    avancerChevaux(positionChevaux);
  }
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

  imageAvatarsJoueurs.push_back(new QPixmap("../Images/cheval1.png"));
  avatarsJoueurs.push_back(new QLabel(this));
  imageAvatarsJoueurs.push_back(new QPixmap("../Images/cheval2.png"));
  avatarsJoueurs.push_back(new QLabel(this));
  imageAvatarsJoueurs.push_back(new QPixmap("../Images/cheval3.png"));
  avatarsJoueurs.push_back(new QLabel(this));
  imageAvatarsJoueurs.push_back(new QPixmap("../Images/cheval4.png"));
  avatarsJoueurs.push_back(new QLabel(this));
  imageAvatarsJoueurs.push_back(new QPixmap("../Images/cheval5.png"));
  avatarsJoueurs.push_back(new QLabel(this));
  imageAvatarsJoueurs.push_back(new QPixmap("../Images/cheval6.png"));
  avatarsJoueurs.push_back(new QLabel(this));
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

  for (int i = 0; i < 100; i++) {
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

bool IHM::partieFinie(QVector<unsigned int> positionChevaux) {
  for (int i = 0; i < imageAvatarsJoueurs.size(); i++) {
    if (positionChevaux[i] == 100)
      return true;
  }
  return false;
}

void IHM::actualiserPositionChevaux(QVector<unsigned int> positionChevaux) {
  for (int i = 0; i < imageAvatarsJoueurs.size(); i++) {
    positionChevaux[i] = positionChevaux[i] + BLEU;
    if (positionChevaux[i] > 100)
      positionChevaux[i] = 100;
  }
}

void IHM::avancerChevaux(QVector<unsigned int> positionChevaux) {
  for (int i = 0; i < imageAvatarsJoueurs.size(); i++) {
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
