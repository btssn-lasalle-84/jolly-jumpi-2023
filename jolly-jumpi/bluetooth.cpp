#include "bluetooth.h"
#include "course.h"
#include "IHM.h"


Bluetooth::Bluetooth(IHM* ihm) :
    QObject(), ihm(ihm), course(nullptr), serveur(nullptr), socket(nullptr), estConnecte(false)
{
    qDebug() << Q_FUNC_INFO;
    demarrerCommunication();
    initialiserCommunication();
    connecterClient();
    connecterSignauxSlots();
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
    if(peripheriqueLocal.isValid())
    {
    peripheriqueLocal.powerOn();
    nomPeripheriqueLocal     = peripheriqueLocal.name();
    adressePeripheriqueLocal = peripheriqueLocal.address().toString();
    qDebug() << Q_FUNC_INFO << "nomPeripheriqueLocal" << nomPeripheriqueLocal
             << "adressePeripheriqueLocal" << adressePeripheriqueLocal;
    peripheriqueLocal.setHostMode(QBluetoothLocalDevice::HostConnectable);
    }
}

void Bluetooth::connecterSignauxSlots()
{
    connect(socket, SIGNAL(abandonPartie(infosTrame)), this, SLOT(course->quitterPartie()));
    connect(socket, SIGNAL(boutonStart(infosTrame)), this, SLOT(course->validerSelection()));
    connect(socket, SIGNAL(encodeurDroite(infosTrame)), this, SLOT(course->selectionnerSuivant()));
    connect(socket, SIGNAL(encodeurGauche(infosTrame)), this, SLOT(course->selectionnerPrecedent()));
    connect(socket, SIGNAL(pointMarque(infosTrame)), this, SLOT(course->quitterPartie()));
    connect(socket, SIGNAL(abandonPartie(infosTrame[NUMERO_TABLE].toInt(),
                                         infosTrame[NUMERO_TROU].toInt(),
                                         infosTrame[COULEUR_ANNEAU].toInt())), this, SLOT(course->avancerChevaux()));
}

void Bluetooth::avancerChevaux()
{
    int  numeroCheval            = (infosTrame[NUMERO_TABLE].toInt()) - 1;
    int  trou                    = infosTrame[COULEUR_ANNEAU].toInt();
    course->actualiserPositionChevaux(numeroCheval, trou);
}

void Bluetooth::connecterClient()
{
    socket = serveur->nextPendingConnection();
    if(!socket)
        return;

    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecterClient()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(lireTrame()));
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
        QStringList trames = trame.split(FIN_TRAME, Qt::SkipEmptyParts);
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

void Bluetooth::envoyerTrame(QString trame)
{
    qDebug() << Q_FUNC_INFO << estConnecte;
    if (estConnecte)
    {
        qDebug() << Q_FUNC_INFO << trame;
        socket->write(trame.toLocal8Bit());
    }
    else
        qDebug() << Q_FUNC_INFO << "Bluetooth non connecté";
}

void Bluetooth::envoyerTrameConnection()
{
    qDebug() << Q_FUNC_INFO;
    /*trame = ENTETE_TRAME + DELIMITEUR_TRAME + CONNECTE + DELIMITEUR_TRAME + FIN_TRAME;
    envoyerTrame(trame);*/
}

void Bluetooth::envoyerTrameDebutCourse()
{
    qDebug() << Q_FUNC_INFO;
    /*trame = ENTETE_TRAME + DELIMITEUR_TRAME + DEBUT_COURSE + DELIMITEUR_TRAME + FIN_TRAME;
    envoyerTrame(trame);*/
}

void Bluetooth::envoyerTrameFinCourse()
{
    qDebug() << Q_FUNC_INFO;
    /*QString trame = ENTETE_TRAME + DELIMITEUR_TRAME + FIN_COURSE + DELIMITEUR_TRAME + FIN_TRAME;
    qDebug() << Q_FUNC_INFO << estConnecte;
    envoyerTrame(trame);*/
}

bool Bluetooth::traiterTrame(QStringList infosTrame)
{
    switch(infosTrame[1].toInt())
    {
        case ABANDON:
            emit abandonPartie(infosTrame);
            break;

        case START:
            emit boutonStart(infosTrame);
            break;

        case DROITE:
            emit encodeurDroite(infosTrame);
            break;

        case GAUCHE:
            emit encodeurGauche(infosTrame);
            break;

        case TIR:
            emit pointMarque(infosTrame[NUMERO_TABLE].toInt(),
                             infosTrame[NUMERO_TROU].toInt(),
                             infosTrame[COULEUR_ANNEAU].toInt());
            break;

        default:
            qDebug() << Q_FUNC_INFO << "Warning: trame inconnue";
            return false;
    }

    return true;
}

void Bluetooth::abandonnerPartie()
{
    qDebug() << Q_FUNC_INFO;
    /*
    course->terminerCourse();
    ihm->afficherPageConnexion();
    */
}

void Bluetooth::validerSelection()
{
    // à faire
}

void Bluetooth::selectionnerSuivant()
{
    // à faire
}

void Bluetooth::selectionnerPrecedent()
{
    // à faire
}
