#include "bluetooth.h"

Bluetooth::Bluetooth(QObject *parent) : QObject(parent),
    serveur(nullptr), socket(nullptr)
{
    qDebug() << Q_FUNC_INFO;
}

Bluetooth::~Bluetooth()
{
    qDebug() << Q_FUNC_INFO;
}

void Bluetooth::demarrerCommunication()
{
    if(!peripheriqueLocal.isValid())
        return;

    if(serveur == nullptr)
    {
        qDebug() << Q_FUNC_INFO << "peripheriqueLocal.isValid()" << peripheriqueLocal.isValid();
        serveur = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
        connect(serveur,
                SIGNAL(newConnection()),
                this,
                SLOT(connecterClient()));

        QBluetoothUuid uuid(QBluetoothUuid::Rfcomm);
        // QBluetoothUuid uuid = QBluetoothUuid(serviceUuid);
        serviceInfo = serveur->listen(uuid, serviceNom);
    }
}

void Bluetooth::initialiserCommunication()
{
    if(!peripheriqueLocal.isValid())
        return;

    peripheriqueLocal.powerOn();
    qDebug() << Q_FUNC_INFO << "nomPeripheriqueLocal" << peripheriqueLocal.name();
             << "adressePeripheriqueLocal" << peripheriqueLocal.address().toString();
    peripheriqueLocal.setHostMode(QBluetoothLocalDevice::HostConnectable);
}

void Bluetooth::connecterClient()
{
    socket = serveur->nextPendingConnection();
    if(!socket)
        return;

    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecterClient()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(lireDonnees()));

    emit clientConnecte();
}

void Bluetooth::deconnecterClient()
{
    qDebug() << Q_FUNC_INFO;
    emit clientDeconnecte();
}

void Bluetooth::lireDonnees()
{
    QByteArray donnees;

    donnees += socket->readAll();
    qDebug() << Q_FUNC_INFO << donnees;
}
