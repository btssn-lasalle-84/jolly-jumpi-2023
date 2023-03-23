#include "IHM.h"
#include "chevaux.h"
#include "ui_IHM.h"

IHM::IHM(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IHM)
{
    ui->setupUi(this);
}

IHM::~IHM()
{
    delete ui;
}

void IHM::jouer()
{
   chevaux chevaux;
   chevaux.avancerLesChevaux(0);
}
