#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <QObject>
#include <QtBluetooth>
#include <QBluetoothLocalDevice>
#include <QDebug>

static const QString serviceUuid(
  QStringLiteral("00001101-0000-1000-8000-00805F9B34FC")); // à remplacer

static const QString serviceNom(QStringLiteral("EcranJJ"));

class Bluetooth : public QObject
{
    Q_OBJECT

private:
    QBluetoothLocalDevice peripheriqueLocal;
    QBluetoothServer* serveur;
    QBluetoothSocket* socket;
    QBluetoothServiceInfo serviceInfo;

public:
    Bluetooth(QObject *parent = nullptr);
    ~Bluetooth();

    void demarrerCommunication();
    void initialiserCommunication();

signals:
  void clientConnecte();
  void clientDeconnecte();
  void empochage(int numeroTable, int numeroPoche, int couleur);
  void nomsJoueurs(int numeroTable, QString nomJoueur1, QString nomJoueur2);
  void changementJoueur(int numeroTable, int changementJoueur);

public slots:
    void connecterClient();
    void deconnecterClient();
    void lireDonnees();

};

#endif // BLUETOOTH_H
