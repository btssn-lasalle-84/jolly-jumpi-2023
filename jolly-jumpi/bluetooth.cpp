#include "bluetooth.h"
#include "course.h"
#include "IHM.h"

Bluetooth::Bluetooth(IHM* ihm) :
    QObject(), ihm(ihm), course(nullptr), socket(nullptr), abandon(false)
{
#ifdef SIMULATEUR
    adresseESP32 = ADRESSE_ESP32_SIMULATEUR;
    qDebug() << Q_FUNC_INFO << "SIMULATEUR";
#else
    adresseESP32 = ADRESSE_ESP32_JOLLY_JUMPI;
    qDebug() << Q_FUNC_INFO << "PAS SIMULATEUR";
#endif
}

Bluetooth::~Bluetooth()
{
    qDebug() << Q_FUNC_INFO;
}

bool Bluetooth::getAbandon() const
{
    return abandon;
}

void Bluetooth::setCourse(Course* course)
{
    this->course = course;
}

void Bluetooth::setAbandon(bool abandon)
{
    this->abandon = abandon;
}

void Bluetooth::initialiserCommunication()
{
    if(peripheriqueLocal.isValid())
    {
        peripheriqueLocal.powerOn();
        nomPeripheriqueLocal     = peripheriqueLocal.name();
        adressePeripheriqueLocal = peripheriqueLocal.address().toString();
        qDebug() << Q_FUNC_INFO << "nomPeripheriqueLocal"
                 << nomPeripheriqueLocal << "adressePeripheriqueLocal"
                 << adressePeripheriqueLocal;
        // peripheriqueLocal.setHostMode(QBluetoothLocalDevice::HostConnectable);

        agentDecouverteBluetooth = new QBluetoothDeviceDiscoveryAgent(this);

        if(socket == nullptr)
        {
            socket =
              new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
        }
        connecterSignauxSlots();

        agentDecouverteBluetooth->start();
    }
}

void Bluetooth::connecterSignauxSlots()
{
    connect(agentDecouverteBluetooth,
            SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this,
            SLOT(gererPeripherique(QBluetoothDeviceInfo)));
    connect(this, SIGNAL(peripheriqueTrouve()), this, SLOT(connecter()));
    connect(socket, SIGNAL(connected()), ihm, SLOT(gererEtatConnexion()));
    connect(socket, SIGNAL(disconnected()), ihm, SLOT(gererEtatDeconnexion()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(lireTrame()));
    connect(this, SIGNAL(abandonPartie()), course, SLOT(terminerCourse()));
    connect(this, SIGNAL(boutonStart()), ihm, SLOT(validerSelection()));
    connect(this, SIGNAL(encodeurDroite()), ihm, SLOT(selectionnerSuivant()));
    connect(this, SIGNAL(encodeurGauche()), ihm, SLOT(selectionnerPrecedent()));
    connect(this,
            SIGNAL(pointMarque(int, int)),
            course,
            SLOT(actualiserPositionChevaux(int, int)));
}

void Bluetooth::connecter()
{
    qDebug() << Q_FUNC_INFO << "adresse" << peripheriqueDistant.address();
    QBluetoothUuid uuid = QBluetoothUuid(QBluetoothUuid::SerialPort);
    socket->connectToService(peripheriqueDistant.address(), uuid);
    socket->open(QIODevice::ReadWrite);
}

void Bluetooth::deconnecter()
{
    qDebug() << Q_FUNC_INFO;
    if(socket->isOpen())
    {
        socket->close();
    }
}

void Bluetooth::gererPeripherique(QBluetoothDeviceInfo peripherique)
{
    qDebug() << Q_FUNC_INFO << "nom" << peripherique.name() << "adresse"
             << peripherique.address();
    if(peripherique.address().toString() == adresseESP32)
    {
        peripheriqueDistant = peripherique;
        agentDecouverteBluetooth->stop();
        emit peripheriqueTrouve();
    }
}

void Bluetooth::lireTrame()
{
    QByteArray donnees;

    donnees = socket->readAll();
    donneesRecues += QString(donnees.data());
    qDebug() << Q_FUNC_INFO << "donneesRecues" << donneesRecues;

    if(donneesRecues.contains(ENTETE_TRAME) &&
       donneesRecues.endsWith(FIN_TRAME))
    {
        QStringList trames =
          donneesRecues.split(FIN_TRAME, QString::SkipEmptyParts);
        qDebug() << Q_FUNC_INFO << "trames" << trames;

        for(int i = 0; i < trames.count(); ++i)
        {
            if(trames[i].startsWith(ENTETE_TRAME))
            {
                traiterTrame(trames[i]);
            }
        }
        donneesRecues.clear();
    }
}

void Bluetooth::envoyerTrame(QString trame)
{
    qDebug() << Q_FUNC_INFO << "estConnecte" << ihm->estConnecte();
    if(socket != nullptr && ihm->estConnecte())
    {
        qDebug() << Q_FUNC_INFO << "trame" << trame;
        int retour = socket->write(trame.toLocal8Bit());
        qDebug() << Q_FUNC_INFO << "retour" << retour;
    }
}

void Bluetooth::envoyerTrameConnexion()
{
    qDebug() << Q_FUNC_INFO;
    QString trame =
      ENTETE_TRAME + DELIMITEUR_TRAME + CONNECTE + DELIMITEUR_TRAME + FIN_TRAME;
    envoyerTrame(trame);
}

void Bluetooth::envoyerTrameDebutCourse()
{
    qDebug() << Q_FUNC_INFO;
    QString trame = ENTETE_TRAME + DELIMITEUR_TRAME + DEBUT_COURSE +
                    DELIMITEUR_TRAME + FIN_TRAME;
    envoyerTrame(trame);
}

void Bluetooth::envoyerTrameFinCourse()
{
    qDebug() << Q_FUNC_INFO;
    QString trame = ENTETE_TRAME + DELIMITEUR_TRAME + FIN_COURSE +
                    DELIMITEUR_TRAME + FIN_TRAME;
    envoyerTrame(trame);
}

bool Bluetooth::traiterTrame(QString trame)
{
    QStringList champsTrame = trame.split(DELIMITEUR_TRAME);
    qDebug() << Q_FUNC_INFO << "champsTrame" << champsTrame;
    switch(champsTrame[TYPE_TRAME].at(0).toLatin1())
    {
        case ABANDON:
            qDebug() << Q_FUNC_INFO << "ABANDON";
            abandon = true;
            emit abandonPartie();
            break;
        case START:
            qDebug() << Q_FUNC_INFO << "START";
            emit boutonStart();
            break;
        case DROITE:
            qDebug() << Q_FUNC_INFO << "DROITE";
            emit encodeurDroite();
            break;
        case GAUCHE:
            qDebug() << Q_FUNC_INFO << "GAUCHE";
            emit encodeurGauche();
            break;
        case TIR:
            qDebug() << Q_FUNC_INFO << "TIR";
            // $JJ;t;{numeroTable};{positionTrou};{couleurAnneau};\r\n
            emit pointMarque(champsTrame[NUMERO_TABLE].toInt() - 1,
                             /*champsTrame[NUMERO_TROU].toInt(),*/
                             champsTrame[COULEUR_ANNEAU].toInt());
            break;
        default:
            qDebug() << Q_FUNC_INFO << "trame inconnue";
            return false;
    }

    return true;
}
