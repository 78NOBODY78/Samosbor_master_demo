#ifndef GAMEMODEWINDOW_H
#define GAMEMODEWINDOW_H

#include <QWidget>
#include "playermodewindow.h"  // Подключаем окно режима игрока

namespace Ui {
class GameModeWindow;
}

// Окно выбора режима игры
class GameModeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameModeWindow(QWidget *parent = nullptr);
    ~GameModeWindow();

private:
    Ui::GameModeWindow *ui;
    PlayerModeWindow *playerModeWindow = nullptr;  // Указатель на окно режима игрока

private slots:
    void onPlayerModeClicked();  // Обработчик кнопки "Игрок"
    void onMasterModeClicked();  // Обработчик кнопки "Мастер"
    void onBackClicked();        // Обработчик кнопки "Назад"
};

#endif // GAMEMODEWINDOW_H
