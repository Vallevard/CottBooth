#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QWidget>

namespace Ui {
class StartScreen;
}

class StartScreen : public QWidget
{
    Q_OBJECT

public:
    explicit StartScreen(QWidget *parent = 0);
    ~StartScreen();

private:
    Ui::StartScreen *ui;


public slots:
    void btnSettingsClicked();
};

#endif // STARTSCREEN_H
