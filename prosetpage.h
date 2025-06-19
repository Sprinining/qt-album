#ifndef PROSETPAGE_H
#define PROSETPAGE_H

#include <QWizardPage>

namespace Ui {
class ProSetPage;
}

class ProSetPage : public QWizardPage {
    Q_OBJECT

  public:
    explicit ProSetPage(QWidget* parent = nullptr);
    ~ProSetPage();

  private:
    Ui::ProSetPage* ui;
};

#endif // PROSETPAGE_H
