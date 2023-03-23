#ifndef IHM_H
#define IHM_H

#include <QWidget>

namespace Ui { class IHM; }

class IHM : public QWidget
{
    Q_OBJECT
public:
    IHM(QWidget *parent = nullptr);
    ~IHM();

    void jouer();

private:
    Ui::IHM *ui;
};
#endif // IHM_H
