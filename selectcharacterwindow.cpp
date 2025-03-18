#include "selectcharacterwindow.h"
#include "ui_selectcharacterwindow.h"
#include "playermodewindow.h"
#include <QMessageBox>

// Конструктор
SelectCharacterWindow::SelectCharacterWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::SelectCharacterWindow)
{
    ui->setupUi(this);

    // Загружаем персонажей
    loadCharacters();

    // Подключаем обработчики
    connect(ui->comboBoxCharacters, SIGNAL(currentIndexChanged(int)), this, SLOT(onCharacterSelected(int)));
    connect(ui->pushButtonSave, &QPushButton::clicked, this, &SelectCharacterWindow::onSaveCharacter);
    connect(ui->pushButton, &QPushButton::clicked, this, &SelectCharacterWindow::onDeleteCharacter);
    connect(ui->pushButtonBack, &QPushButton::clicked, this, &SelectCharacterWindow::onBackClicked);
    connect(ui->lineEditXp, &QLineEdit::editingFinished, this, &SelectCharacterWindow::onXpChanged);
}

// Деструктор
SelectCharacterWindow::~SelectCharacterWindow()
{
    delete ui;
}

// Загрузка персонажей из JSON-файла
void SelectCharacterWindow::loadCharacters()
{
    QFile file("character.json");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить персонажей.");
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isObject()) {
        QMessageBox::warning(this, "Ошибка", "Некорректный формат данных.");
        return;
    }

    loadedCharacters = doc.object();
    ui->comboBoxCharacters->clear(); // Очищаем список перед заполнением

    // Фильтруем только имена персонажей
    QStringList characterNames;
    for (const QString &key : loadedCharacters.keys()) {
        if (loadedCharacters[key].isObject()) {  // Проверяем, что значение - объект (то есть персонаж)
            characterNames.append(key);
        }
    }

    ui->comboBoxCharacters->addItems(characterNames); // Добавляем в выпадающий список

    if (!characterNames.isEmpty()) {
        ui->comboBoxCharacters->setCurrentIndex(0); // Выбираем первого персонажа
        updateCharacterInfo(characterNames.first()); // Загружаем данные первого персонажа
    }
}

void SelectCharacterWindow::onCharacterSelected(int index)
{
    if (index < 0) return; // Защита от ошибок

    QString selectedName = ui->comboBoxCharacters->itemText(index);
    updateCharacterInfo(selectedName);
}


// Заполнение полей при выборе персонажа
void SelectCharacterWindow::updateCharacterInfo(const QString &name)
{
    if (!loadedCharacters.contains(name))
        return;

    QJsonObject character = loadedCharacters[name].toObject();

    ui->labClass->setText(character["Класс"].toString());
    ui->labelLvL->setText(QString::number(character["Уровень"].toInt()));  // ✅ Загружаем уровень
    ui->labelXp->setText(QString::number(character["Опыт"].toInt()));
    ui->spinBoxStrenght->setValue(character["Сила"].toInt());
    ui->spinBoxAgility->setValue(character["Ловкость"].toInt());
    ui->spinBoxIntelligence->setValue(character["Интеллект"].toInt());
    ui->spinBoxWeaponSkill->setValue(character["Навык_Оружие"].toInt());
    ui->spinBoxMedicineSkill->setValue(character["Навык_Медицина"].toInt());
    ui->spinBoxMechanicSkill->setValue(character["Навык_Механика"].toInt());
    ui->spinBoxElectronicsSkill->setValue(character["Навык_Электроника"].toInt());

    // Загружаем особенности
    QJsonArray traitsArray = character["Особенности"].toArray();
    ui->labelTraits1->setText(traitsArray.size() > 0 ? traitsArray[0].toString() : "-");
    ui->labelTraits2->setText(traitsArray.size() > 1 ? traitsArray[1].toString() : "-");
    ui->labelTraits3->setText(traitsArray.size() > 2 ? traitsArray[2].toString() : "-");

    updatePointsLeft();
}

// Обновление опыта
void SelectCharacterWindow::onXpChanged()
{
    int xpChange = ui->lineEditXp->text().toInt();
    int currentXP = ui->labelXp->text().toInt();
    currentXP += xpChange; // Добавляем/вычитаем опыт

    if (currentXP < 0) currentXP = 0; // Опыт не может быть отрицательным

    ui->labelXp->setText(QString::number(currentXP));
    ui->lineEditXp->clear(); // Очищаем поле ввода

    updateLevel(); // ✅ Проверяем, можно ли повысить уровень
}


// Логика обновления опыта и уровня
void SelectCharacterWindow::updateExperience(int xpChange)
{
    QString name = ui->comboBoxCharacters->currentText();
    if (!loadedCharacters.contains(name))
        return;

    QJsonObject character = loadedCharacters[name].toObject();
    int experience = character["Опыт"].toInt();
    experience += xpChange;
    if (experience < 0) experience = 0;

    character["Опыт"] = experience;
    loadedCharacters[name] = character;

    ui->labelXp->setText(QString::number(experience));
    updateCharacterInfo(name);
}

void SelectCharacterWindow::updateLevel()
{
    int xp = ui->labelXp->text().toInt();  // Текущий опыт
    int level = ui->labelLvL->text().toInt();  // Текущий уровень
    int previousLevel = level; // Сохраняем предыдущий уровень

    int requiredXP = 150; // Опыт для 1-го уровня
    for (int i = 1; i < level; ++i) {
        requiredXP *= 2;  // Увеличиваем в 2 раза на каждом уровне
    }

    while (xp >= requiredXP) {
        xp -= requiredXP; // Вычитаем использованный опыт
        level++;  // Повышаем уровень
        requiredXP *= 2; // Следующий уровень требует больше опыта

        // Каждые 2 уровня добавляем 1 очко характеристик
        if (level % 2 == 0) {
            pointsLeft++;
        }
    }

    // Обновляем интерфейс
    ui->labelLvL->setText(QString::number(level));
    ui->labelXp->setText(QString::number(xp));

    // Если уровень изменился, обновляем очки характеристик
    if (level > previousLevel) {
        updatePointsLeft();
    }
}

// Обновление доступных очков характеристик
void SelectCharacterWindow::updatePointsLeft()
{
    int total = ui->spinBoxStrenght->value() + ui->spinBoxAgility->value() + ui->spinBoxIntelligence->value();
    int level = ui->labelLvL->text().toInt();
    int maxPoints = level / 2;  // 1 очко на каждые 2 уровня

    pointsLeft = maxPoints - total;
    if (pointsLeft < 0) pointsLeft = 0; // ❗ Очки не могут быть отрицательными

    ui->labelPointsLeft->setText("Осталось очков: " + QString::number(pointsLeft));

    // Блокируем spinBox'ы, если очков нет
    ui->spinBoxStrenght->setEnabled(pointsLeft > 0 || ui->spinBoxStrenght->value() > 0);
    ui->spinBoxAgility->setEnabled(pointsLeft > 0 || ui->spinBoxAgility->value() > 0);
    ui->spinBoxIntelligence->setEnabled(pointsLeft > 0 || ui->spinBoxIntelligence->value() > 0);
}

// Сохранение изменений
void SelectCharacterWindow::onSaveCharacter()
{
    QString name = ui->comboBoxCharacters->currentText();
    if (!loadedCharacters.contains(name))
        return;

    QJsonObject character;
    character["Класс"] = ui->labClass->text();
    character["Уровень"] = ui->labelLvL->text().toInt();  // ✅ Сохраняем уровень
    character["Опыт"] = ui->labelXp->text().toInt();
    character["Сила"] = ui->spinBoxStrenght->value();
    character["Ловкость"] = ui->spinBoxAgility->value();
    character["Интеллект"] = ui->spinBoxIntelligence->value();
    character["Навык_Оружие"] = ui->spinBoxWeaponSkill->value();
    character["Навык_Медицина"] = ui->spinBoxMedicineSkill->value();
    character["Навык_Механика"] = ui->spinBoxMechanicSkill->value();
    character["Навык_Электроника"] = ui->spinBoxElectronicsSkill->value();

    // Сохраняем особенности персонажа
    QJsonArray traitsArray;
    traitsArray.append(ui->labelTraits1->text());
    traitsArray.append(ui->labelTraits2->text());
    traitsArray.append(ui->labelTraits3->text());
    character["Особенности"] = traitsArray;

    loadedCharacters[name] = character;
    QJsonDocument doc(loadedCharacters);
    QFile file("character.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }

    QMessageBox::information(this, "Сохранение", "Персонаж сохранён.");
}

// Удаление персонажа
void SelectCharacterWindow::onDeleteCharacter()
{
    QString name = ui->comboBoxCharacters->currentText();
    loadedCharacters.remove(name);
    onSaveCharacter();
    ui->comboBoxCharacters->removeItem(ui->comboBoxCharacters->currentIndex());
}

// Кнопка "Назад"
void SelectCharacterWindow::onBackClicked()
{
    this->close(); // Закрываем текущее окно

    // Создаем новое окно PlayerModeWindow и показываем его
    PlayerModeWindow *playerWindow = new PlayerModeWindow();
    playerWindow->show();
}




