#ifndef IHM_H
#define IHM_H

#include <QtWidgets>

namespace Ui
{
class IHM;
}

class IHM : public QWidget
{
    Q_OBJECT
  public:
    IHM(QWidget* parent = nullptr);
    ~IHM();

    void jouer();

  private:
    Ui::IHM* ui;

    void instancierWidgets();
    void initialiserWidgets();
    void positionnerWidgets();
    void connecterSignauxSlots();
    void initialiserFenetre();
};

#endif // IHM_H
