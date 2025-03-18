#include "gamemodewindow.h"
#include "ui_gamemodewindow.h"
#include "mainwindow.h"  // Подключаем главное окно
#include "playermodewindow.h"  // Подключаем окно режима игрока

GameModeWindow::GameModeWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameModeWindow)
    , playerModeWindow(nullptr)  // Инициализируем указатель
{
    ui->setupUi(this);

    // Подключаем кнопки к обработчикам
    connect(ui->pushButtonPlayer, &QPushButton::clicked, this, &GameModeWindow::onPlayerModeClicked);
    connect(ui->pushButtonMaster, &QPushButton::clicked, this, &GameModeWindow::onMasterModeClicked);
    connect(ui->pushButtBack, &QPushButton::clicked, this, &GameModeWindow::onBackClicked);
}

GameModeWindow::~GameModeWindow()
{
    delete ui;
}

// Обработчик кнопки "Игрок" — открывает окно режима игрока
void GameModeWindow::onPlayerModeClicked()
{
    if (!playerModeWindow) {
        playerModeWindow = new PlayerModeWindow(nullptr);  // Создаем, если ещё нет
    }
    playerModeWindow->show();
    this->hide();  // Скрываем текущее окно
}

// Обработчик кнопки "Мастер" — (реализация ожидается)
void GameModeWindow::onMasterModeClicked()
{
    // TODO: Добавить переход в режим мастера
}

// Обработчик кнопки "Назад" — возвращает в главное окно
void GameModeWindow::onBackClicked()
{
    if (parentWidget()) {
        parentWidget()->show();  // Показываем главное окно
    }
    this->close();  // Закрываем окно выбора режима
}
