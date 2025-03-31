#include "gamemodewindow.h"
#include "ui_gamemodewindow.h"
#include "mainwindow.h"
#include "playermodewindow.h"
#include "mastercontrolwindow.h"

GameModeWindow::GameModeWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameModeWindow)
    , playerModeWindow(nullptr)
    , masterWindow(nullptr)
{
    ui->setupUi(this);

    connect(ui->pushButtonPlayer, &QPushButton::clicked, this, &GameModeWindow::onPlayerModeClicked);
    connect(ui->pushButtonMaster, &QPushButton::clicked, this, &GameModeWindow::onMasterModeClicked);
    connect(ui->pushButtBack, &QPushButton::clicked, this, &GameModeWindow::onBackClicked);
}

GameModeWindow::~GameModeWindow()
{
    delete playerModeWindow;
    delete masterWindow;
    delete ui;
}

void GameModeWindow::onPlayerModeClicked()
{
    if (!playerModeWindow) {
        playerModeWindow = new PlayerModeWindow(nullptr); // указываем родителя
    }
    playerModeWindow->show();
    this->hide();
}

void GameModeWindow::onMasterModeClicked()
{
    if (!masterWindow) {
        masterWindow = new MasterControlWindow(nullptr);  // ← БЕЗ this
    }
    masterWindow->show();
    this->hide();
}



void GameModeWindow::onBackClicked()
{
    if (parentWidget()) {
        parentWidget()->show();
    }
    this->close();
}
