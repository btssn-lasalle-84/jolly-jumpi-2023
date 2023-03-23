#include "IHM.h"
#include "ui_IHM.h"

IHM::IHM(QWidget* parent) : QWidget(parent), ui(new Ui::IHM)
{
    instancierWidgets();

    initialiserWidgets();

    positionnerWidgets();

    connecterSignauxSlots();

    initialiserFenetre();
}

IHM::~IHM()
{
    delete ui;
}

void IHM::jouer()
{
}

void IHM::instancierWidgets()
{
    ui->setupUi(this);
}

void IHM::initialiserWidgets()
{
}

void IHM::positionnerWidgets()
{
}

void IHM::connecterSignauxSlots()
{
}

void IHM::initialiserFenetre()
{
}
