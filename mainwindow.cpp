#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gamemodewindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gameModeWindow(nullptr)  // Инициализация указателя на окно выбора режима
{
    ui->setupUi(this);

    // Подключаем кнопки к соответствующим слотам
    connect(ui->pushButtonStart, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(ui->pushButtonExit, &QPushButton::clicked, this, &MainWindow::onExitClicked);
}

MainWindow::~MainWindow()
{
    delete ui;  // Освобождаем память, выделенную под UI
}

// Обработчик кнопки "Начать" — открывает окно выбора режима игры
void MainWindow::onStartClicked()
{
    if (!gameModeWindow) {
        gameModeWindow = new GameModeWindow(nullptr);  // Создаем окно, если его ещё нет
    }

    gameModeWindow->show();  // Показываем окно выбора режима
    this->hide();  // Скрываем главное окно
}

// Обработчик кнопки "Выход" — завершает приложение
void MainWindow::onExitClicked()
{
    QApplication::quit();
}
