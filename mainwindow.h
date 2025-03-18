#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamemodewindow.h"  // Подключаем окно выбора режима

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Главное окно приложения
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartClicked();  // Обработчик кнопки "Начать"
    void onExitClicked();   // Обработчик кнопки "Выход"

private:
    Ui::MainWindow *ui;            // Интерфейс главного окна
    GameModeWindow *gameModeWindow; // Указатель на окно выбора режима
};

#endif // MAINWINDOW_H
