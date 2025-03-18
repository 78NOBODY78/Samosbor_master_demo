#ifndef CREATECHARACTERWINDOW_H
#define CREATECHARACTERWINDOW_H

#include <QWidget>
#include <QStringList>
#include <QMessageBox>  // Для всплывающих сообщений
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include "ui_createcharacterwindow.h"

namespace Ui {
class CreateCharacterWindow;
}

// Окно создания персонажа
class CreateCharacterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CreateCharacterWindow(QWidget *parent = nullptr);
    ~CreateCharacterWindow();

private:
    Ui::CreateCharacterWindow *ui;
    int pointsLeft = 3;  // Очки характеристик
    QStringList selectedTraits;  // Выбранные особенности

    void updatePointsLeft();  // Обновление очков характеристик
    void generateSkills();  // Генерация случайных навыков
    void generateTraits();  // Генерация случайных особенностей
    void showTraitDescription(int index);  // Показ описания особенностей
    void saveCharacter();  // Сохранение персонажа

private slots:
    void onClassChanged();  // Изменение класса
    void onPointsChanged();  // Изменение характеристик
    void onCreateCharacter();  // Создание персонажа
    void onSaveCharacter();  // Сохранение персонажа
    void onBackClicked();  // Возврат назад
    void onTraitsHelp1Clicked();  // Описание первой особенности
    void onTraitsHelp2Clicked();  // Описание второй особенности
    void onTraitsHelp3Clicked();  // Описание третьей особенности
    void onHelpClassClicked();  // Описание выбранного класса
};

#endif // CREATECHARACTERWINDOW_H
