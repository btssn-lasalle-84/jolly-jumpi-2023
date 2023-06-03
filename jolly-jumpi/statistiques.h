#ifndef STATISTIQUES_H
#define STATISTIQUES_H

#include <QObject>
#include <QVector>

class Statistiques : public QObject
{
    Q_OBJECT

  private:
    unsigned int          dureeDeLaPartie;
    QVector<unsigned int> nombreDePointsMarques;
    QVector<unsigned int> nombreBallesRentrees;

  public:
    Statistiques(QObject* parent = nullptr);
    ~Statistiques();
};

#endif // STATISTIQUES_H
