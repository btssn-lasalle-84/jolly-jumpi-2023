#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <QObject>
#include <QtBluetooth>
#include <QBluetoothLocalDevice>
#include <QDebug>

static const QString serviceUuid(
  QStringLiteral("00001101-0000-1000-8000-00805f9b34fb")); // simulateur

static const QString serviceNom(QStringLiteral("jolly-jumpi")); // simulateur
// static const QString serviceNom(QStringLiteral("Jolly-Jumpi"));

#define ENTETE_TRAME     QStringLiteral("$JJ")
#define FIN_TRAME        QStringLiteral("\r\n")
#define DELIMITEUR_TRAME QStringLiteral(";")

#define CONNECTE     QStringLiteral("c")
#define DEBUT_COURSE 'd'
#define FIN_COURSE   'f'

#define ABANDON 'a'
#define START   's'
#define DROITE  'd'
#define GAUCHE  'g'
#define TIR     't'

#define NUMERO_TABLE   2
#define NUMERO_TROU    3
#define COULEUR_ANNEAU 4

class Course;
class IHM;

class Bluetooth : public QObject
{
    Q_OBJECT

  private:
    IHM*    ihm;
    Course* course;

    QBluetoothLocalDevice peripheriqueLocal;
    QBluetoothServer*     serveur;
    QBluetoothSocket*     socket;
    QBluetoothServiceInfo serviceInfo;
    QString               nomPeripheriqueLocal;
    QString               adressePeripheriqueLocal;
    bool                  estConnecte;
    QString               trame;
    QString               trames;
    QStringList           infosTrame;

    void envoyerTrame(QString trame);

  public:
    Bluetooth(IHM* ihm);
    ~Bluetooth();

    void demarrerCommunication();
    void initialiserCommunication();
    void connecterSignauxSlots();
    void lireTrame();
    bool traiterTrame(QStringList infosTrame);
    void envoyerTrameConnection();
    void envoyerTrameDebutCourse();
    void envoyerTrameFinCourse();

  signals:
    void clientDeconnecte();
    void abandonPartie(QStringList infosTrame);
    void boutonStart(QStringList infosTrame);
    void encodeurDroite(QStringList infosTrame);
    void encodeurGauche(QStringList infosTrame);
    void pointMarque(int numeroTable, int numeroTrou, int couleurAnneau);

  public slots:
    void connecterClient();
    void deconnecterClient();
    void abandonnerPartie();
    void validerSelection();
    void selectionnerSuivant();
    void selectionnerPrecedent();
    void avancerChevaux();
};

#endif // BLUETOOTH_H
