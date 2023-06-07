#include "bluetooth.h"

Bluetooth::Bluetooth(QObject* parent) :
    QObject(parent), serveur(nullptr), socket(nullptr)
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
        qDebug() << Q_FUNC_INFO << "peripheriqueLocal.isValid()"
                 << peripheriqueLocal.isValid();
        serveur =
          new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
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
    nomPeripheriqueLocal     = peripheriqueLocal.name();
    adressePeripheriqueLocal = peripheriqueLocal.address().toString();
    qDebug() << Q_FUNC_INFO << "nomPeripheriqueLocal" << nomPeripheriqueLocal
             << "adressePeripheriqueLocal" << adressePeripheriqueLocal;
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

void Bluetooth::lireTrame()
{
    QByteArray donnees;

    donnees = socket->readAll();
    qDebug() << Q_FUNC_INFO << donnees;

    trame += QString(donnees.data());
    qDebug() << Q_FUNC_INFO << trame;

    if(trame.startsWith(ENTETE_TRAME) && trame.endsWith(FIN_TRAME))
    {
        QStringList trames = trame.split(FIN_TRAME, QString::SkipEmptyParts);
        qDebug() << Q_FUNC_INFO << trames;

        for(int i = 0; i < trames.count(); ++i)
        {
            qDebug() << Q_FUNC_INFO << i << trames[i];
            infosTrame = trames[i].split(DELIMITEUR_TRAME);
            traiterTrame(infosTrame);
        }
        trame.clear();
    }
}

bool Bluetooth::traiterTrame(QStringList infosTrame)
{
    switch(infosTrame[1].toInt())
    {
        case ABANDON:
            emit(abandonPartie(infosTrame));
            break;

        case START:
            emit(boutonStart(infosTrame));
            break;

        case DROITE:
            emit(encodeurDroite(infosTrame));
            break;

        case GAUCHE:
            emit(encodeurGauche(infosTrame));
            break;

        case TIR:
            emit(pointMarque(infosTrame[NUMERO_TABLE].toInt(),
                             infosTrame[NUMERO_TROU].toInt(),
                             infosTrame[COULEUR_ANNEAU].toInt()));
            break;

        default:
            qDebug() << Q_FUNC_INFO << "Warning: trame inconnue";
            return false;
    }

    return true;
}
