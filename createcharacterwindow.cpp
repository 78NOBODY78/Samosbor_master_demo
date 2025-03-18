#include <QMessageBox>
#include <QRandomGenerator>
#include <iterator>
#include <QJsonArray>  // ✅ Добавляем недостающий заголовок
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include "createcharacterwindow.h"
#include "ui_createcharacterwindow.h"
#include "character_traits.h"
#include "character_skills.h"
#include "character_classes.h"

// Конструктор окна создания персонажа
CreateCharacterWindow::CreateCharacterWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::CreateCharacterWindow)
{
    ui->setupUi(this);

    // Подключаем кнопки к слотам
    connect(ui->pushButtonCreate, &QPushButton::clicked, this, &CreateCharacterWindow::onCreateCharacter);
    connect(ui->pushButtoSave, &QPushButton::clicked, this, &CreateCharacterWindow::onSaveCharacter);
    connect(ui->pushButtonBack, &QPushButton::clicked, this, &CreateCharacterWindow::onBackClicked);
    connect(ui->pushButtonHelpClass, &QPushButton::clicked, this, &CreateCharacterWindow::onHelpClassClicked);

    connect(ui->pushButtonTraitsHelp1, &QPushButton::clicked, this, &CreateCharacterWindow::onTraitsHelp1Clicked);
    connect(ui->pushButtonTraitsHelp2, &QPushButton::clicked, this, &CreateCharacterWindow::onTraitsHelp2Clicked);
    connect(ui->pushButtonTraitsHelp3, &QPushButton::clicked, this, &CreateCharacterWindow::onTraitsHelp3Clicked);

    connect(ui->spinBoxStrenght, SIGNAL(valueChanged(int)), this, SLOT(onPointsChanged()));
    connect(ui->spinBoxAgility, SIGNAL(valueChanged(int)), this, SLOT(onPointsChanged()));
    connect(ui->spinBoxIntelligence, SIGNAL(valueChanged(int)), this, SLOT(onPointsChanged()));

    // Разрешаем отрицательные значения характеристик
    ui->spinBoxStrenght->setMinimum(-1);
    ui->spinBoxAgility->setMinimum(-1);
    ui->spinBoxIntelligence->setMinimum(-1);
}

// Деструктор
CreateCharacterWindow::~CreateCharacterWindow()
{
    delete ui;
}

// Генерация случайных навыков по выбранному классу
void CreateCharacterWindow::generateSkills()
{
    QString selectedClass = ui->comboBoxClass->currentText();

    ui->labelWeaponSkill->setText(QString::number(CharacterSkills::generateSkill(selectedClass, "Оружие")));
    ui->labelMedicineSkill->setText(QString::number(CharacterSkills::generateSkill(selectedClass, "Медицина")));
    ui->labelMechanicSkill->setText(QString::number(CharacterSkills::generateSkill(selectedClass, "Механика")));
    ui->labelElectronicsSkill->setText(QString::number(CharacterSkills::generateSkill(selectedClass, "Электроника")));
}

// Генерация случайных особенностей персонажа
void CreateCharacterWindow::generateTraits()
{
    auto traits = CharacterTraits::getTraits();
    selectedTraits.clear();

    if (traits.isEmpty()) {
        ui->labelTraits1->setText("-");
        ui->labelTraits2->setText("-");
        ui->labelTraits3->setText("-");
        return;
    }

    while (selectedTraits.size() < 3) {
        int randomIndex = QRandomGenerator::global()->bounded(traits.size());
        auto it = traits.begin();
        std::advance(it, randomIndex);

        if (!selectedTraits.contains(it.key())) {
            selectedTraits.append(it.key());
        }
    }

    ui->labelTraits1->setText(selectedTraits[0]);
    ui->labelTraits2->setText(selectedTraits[1]);
    ui->labelTraits3->setText(selectedTraits[2]);
}

// Обновление очков характеристик
void CreateCharacterWindow::updatePointsLeft()
{
    int total = ui->spinBoxStrenght->value() + ui->spinBoxAgility->value() + ui->spinBoxIntelligence->value();
    pointsLeft = 3 - total;

    ui->labelPointsLeft->setText(pointsLeft < 0
                                     ? "Заимствовано: " + QString::number(-pointsLeft)
                                     : "Осталось очков: " + QString::number(pointsLeft));

    ui->spinBoxStrenght->setMaximum(pointsLeft + ui->spinBoxStrenght->value());
    ui->spinBoxAgility->setMaximum(pointsLeft + ui->spinBoxAgility->value());
    ui->spinBoxIntelligence->setMaximum(pointsLeft + ui->spinBoxIntelligence->value());
}

// Обработчик кнопки "Создать персонажа"
void CreateCharacterWindow::onCreateCharacter()
{
    generateSkills();
    generateTraits();
}

// Обработчик кнопки "Сохранить персонажа"
void CreateCharacterWindow::onSaveCharacter()
{
    QString characterName = ui->lineEditName->text();
    if (characterName.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Имя персонажа не может быть пустым.");
        return;
    }

    // Загружаем существующий JSON-файл
    QFile file("character.json");
    QJsonObject allCharacters;

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isObject()) {
            allCharacters = doc.object(); // Загружаем всех персонажей
        }
    }

    // Создаем JSON-объект персонажа
    QJsonObject characterData;
    characterData["Класс"] = ui->comboBoxClass->currentText();
    characterData["Уровень"] = 0;  // Начальный уровень
    characterData["Опыт"] = 0;  // Начальный опыт
    characterData["Сила"] = ui->spinBoxStrenght->value();
    characterData["Ловкость"] = ui->spinBoxAgility->value();
    characterData["Интеллект"] = ui->spinBoxIntelligence->value();
    characterData["Навык_Оружие"] = ui->labelWeaponSkill->text().toInt();
    characterData["Навык_Медицина"] = ui->labelMedicineSkill->text().toInt();
    characterData["Навык_Механика"] = ui->labelMechanicSkill->text().toInt();
    characterData["Навык_Электроника"] = ui->labelElectronicsSkill->text().toInt();

    // Сохраняем особенности персонажа
    QJsonArray traitsArray;
    for (const QString &trait : selectedTraits) {
        traitsArray.append(trait);
    }
    characterData["Особенности"] = traitsArray;

    // Добавляем нового персонажа в общий JSON-объект
    allCharacters[characterName] = characterData;

    // Конвертируем в JSON-документ
    QJsonDocument jsonDoc(allCharacters);

    // Открываем файл для записи
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить персонажа.");
        return;
    }

    file.write(jsonDoc.toJson());
    file.close();

    QMessageBox::information(this, "Сохранение", "Персонаж успешно сохранён в character.json!");
}



// Обработчик кнопки "Назад"
void CreateCharacterWindow::onBackClicked()
{
    emit this->destroyed();  // Сообщаем о закрытии окна
    this->close();
}


// Вспомогательные кнопки "?" для описания особенностей персонажа
void CreateCharacterWindow::onTraitsHelp1Clicked()
{
    QMessageBox::information(this, "Описание", CharacterTraits::getTraits().value(selectedTraits[0], "Нет описания"));
}

void CreateCharacterWindow::onTraitsHelp2Clicked()
{
    QMessageBox::information(this, "Описание", CharacterTraits::getTraits().value(selectedTraits[1], "Нет описания"));
}

void CreateCharacterWindow::onTraitsHelp3Clicked()
{
    QMessageBox::information(this, "Описание", CharacterTraits::getTraits().value(selectedTraits[2], "Нет описания"));
}

// Описание класса при нажатии на кнопку "?" !!!!!Дополнить описание классов!!!!!

void CreateCharacterWindow::onHelpClassClicked()
{
    QMap<QString, QString> classDescriptions = {
        {"Офицер Службы Безопасности", "Хорош в оружии."},
        {"Медик", "Отличные навыки лечения."},
        {"Инженер", "Разбирается в электронике."},
        {"Механик", "Отлично чинит технику."},
        {"Помощник", "Универсальный боец."}
    };

    QMessageBox::information(this, "Описание класса",
                             classDescriptions.value(ui->comboBoxClass->currentText(), "Описание отсутствует"));
}

// Обработчик изменения класса персонажа
void CreateCharacterWindow::onClassChanged()
{
    generateSkills();
}

// Обработчик изменения
void CreateCharacterWindow::onPointsChanged()
{
    updatePointsLeft();  // Просто пересчитываем очки при изменении характеристик
}
