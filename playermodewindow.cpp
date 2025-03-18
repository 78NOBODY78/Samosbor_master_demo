#include "playermodewindow.h"
#include "ui_playermodewindow.h"
#include "gamemodewindow.h"
#include "createcharacterwindow.h"
#include "selectcharacterwindow.h"

PlayerModeWindow::PlayerModeWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayerModeWindow)
{
    ui->setupUi(this);

    // Подключаем кнопки
    connect(ui->pushButtonCreate, &QPushButton::clicked, this, &PlayerModeWindow::onCreateCharacterClicked);
    connect(ui->pushButtonSelect, &QPushButton::clicked, this, &PlayerModeWindow::onSelectCharacterClicked);
    connect(ui->pushButtonBack, &QPushButton::clicked, this, &PlayerModeWindow::onBackClicked);
}

PlayerModeWindow::~PlayerModeWindow()
{
    delete ui;
}

// Реализация кнопки "Создать персонажа"
void PlayerModeWindow::onCreateCharacterClicked()
{
    if (!characterWindow)
    {
        characterWindow = new CreateCharacterWindow();  // ❌ Убираем передачу `this`
        characterWindow->setAttribute(Qt::WA_DeleteOnClose);

        // Связываем событие закрытия с повторным показом PlayerModeWindow
        connect(characterWindow, &QWidget::destroyed, this, [this]() {
            this->show();
        });
    }
    characterWindow->show();
    this->hide();
}


// Реализация кнопки "Выбрать персонажа"
void PlayerModeWindow::onSelectCharacterClicked()
{
    SelectCharacterWindow *selectCharacterWindow = new SelectCharacterWindow();
    selectCharacterWindow->show();
    this->hide(); // Скрываем текущее окно, чтобы избежать дублирования
}

// Реализация кнопки "Назад"
void PlayerModeWindow::onBackClicked()
{
    GameModeWindow *gameModeWin = new GameModeWindow();
    gameModeWin->show();
    this->close();
}
