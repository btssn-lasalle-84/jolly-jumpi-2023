#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <QObject>
#include <QString>
#include <QtBluetooth>
#include <QBluetoothLocalDevice>
#include <QDebug>

#define ADRESSE_ESP32_JOLLY_JUMPI QString("")
#define NOM_ESP32_JOLLY_JUMPI     QString("JOLLY-JUMPI")

#define ADRESSE_ESP32_SIMULATEUR QString("3C:71:BF:6A:F5:D2")
#define NOM_ESP32_SIMULATEUR     QString("jolly-jumpi")

#define ENTETE_TRAME     QString("$JJ")
#define FIN_TRAME        QString("\r\n")
#define DELIMITEUR_TRAME QString(";")

#define CONNECTE     QString("c")
#define DEBUT_COURSE QString("d")
#define FIN_COURSE   QString("f")

#define ABANDON 'a'
#define START   's'
#define DROITE  'd'
#define GAUCHE  'g'
#define TIR     't'

#define TYPE_TRAME     1
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

    QBluetoothLocalDevice           peripheriqueLocal;
    QBluetoothDeviceInfo            peripheriqueDistant;
    QBluetoothSocket*               socket;
    QBluetoothDeviceDiscoveryAgent* agentDecouverteBluetooth;
    QString                         nomPeripheriqueLocal;
    QString                         adressePeripheriqueLocal;
    QString                         donneesRecues;

    bool traiterTrame(QString trame);
    void envoyerTrame(QString trame);

  public:
    Bluetooth(IHM* ihm);
    ~Bluetooth();

    void setCourse(Course* course);
    void initialiserCommunication();
    void connecterSignauxSlots();
    void deconnecter();
    void envoyerTrameConnexion();
    void envoyerTrameDebutCourse();
    void envoyerTrameFinCourse();

  public slots:
    void gererPeripherique(QBluetoothDeviceInfo peripherique);
    void connecter();
    void lireTrame();

  signals:
    void peripheriqueTrouve();
    void abandonPartie();
    void boutonStart();
    void encodeurDroite();
    void encodeurGauche();
    void pointMarque(int numeroTable, int numeroTrou /*, int couleurAnneau*/);
};

#endif // BLUETOOTH_H
