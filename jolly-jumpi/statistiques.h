#ifndef STATISTIQUES_H
#define STATISTIQUES_H

#include <QObject>
#include <QVector>

#define AUCUN_JOUEUR -1

class Course;
class IHM;

class Statistiques : public QObject
{
    Q_OBJECT

  private:
    IHM*    ihm;
    Course* course;

    int                   nbChevaux;
    QVector<unsigned int> classement;
    int                   positionClassement;
    float                 dureeDeLaPartie;
    QVector<unsigned int> nombreTirs;
    QVector<unsigned int> nombrePoints;
    int                   joueurGagnant;

    int determinerJoueurSuivant();

  public:
    Statistiques(IHM* ihm);
    ~Statistiques();

    void                  setCourse(Course* course);
    int                   getJoueurGagnant() const;
    QVector<unsigned int> getClassement() const;
    QVector<unsigned int> getNombreTirs() const;
    QVector<unsigned int> getNombrePoints() const;
    float                 getDureeDeLaPartie() const;

    void setJoueurGagnant(int);
    void setPositionClassement(int);
    void setDureeDeLaPartie(float);
    void setNombreTirs(QVector<unsigned int>, int);

    void determinerClassement();
    void afficherResultats();
    void afficherResultatJoueurSuivant();
};

#endif // STATISTIQUES_H
