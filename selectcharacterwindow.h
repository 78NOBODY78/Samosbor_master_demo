#ifndef SELECTCHARACTERWINDOW_H
#define SELECTCHARACTERWINDOW_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QLineEdit>

namespace Ui {
class SelectCharacterWindow;
}

class SelectCharacterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SelectCharacterWindow(QWidget *parent = nullptr);
    ~SelectCharacterWindow();

private:
    Ui::SelectCharacterWindow *ui;
    QJsonObject loadedCharacters;  // Хранит загруженных персонажей
    int experienceThreshold = 150; // Базовое значение опыта для 1 уровня
    int pointsLeft = 0;  // Остаток очков характеристик

    void loadCharacters();  // Загрузить персонажей из файла
    void updateCharacterInfo(const QString &name);  // Заполнить поля при выборе
    void updateExperience(int xpChange);  // Обновить опыт и уровень
    void updatePointsLeft();  // Обновить доступные очки характеристик
    void updateLevel();  // Метод обновления уровня
private slots:
    void onCharacterSelected(int index);  // Выбор персонажа
    void onSaveCharacter();  // Сохранение изменений
    void onDeleteCharacter();  // Удаление персонажа
    void onBackClicked();  // Вернуться в PlayerModeWindow
    void onXpChanged();  // Обновление опыта
};

#endif // SELECTCHARACTERWINDOW_H
