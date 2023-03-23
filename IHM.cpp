#include "IHM.h"
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
