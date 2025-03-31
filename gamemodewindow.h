#ifndef GAMEMODEWINDOW_H
#define GAMEMODEWINDOW_H

#include <QWidget>
#include "playermodewindow.h"
#include "mastercontrolwindow.h"

namespace Ui {
class GameModeWindow;
}

class GameModeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameModeWindow(QWidget *parent = nullptr);
    ~GameModeWindow();

private slots:
    void onPlayerModeClicked();
    void onMasterModeClicked();
    void onBackClicked();

private:
    Ui::GameModeWindow *ui;
    PlayerModeWindow *playerModeWindow = nullptr;
    MasterControlWindow *masterWindow = nullptr;
};

#endif // GAMEMODEWINDOW_H
