#include "character_skills.h"
#include <QRandomGenerator>

// Генерирует случайное значение навыка в зависимости от класса персонажа
int CharacterSkills::generateSkill(const QString& className, const QString& skill)
{
    auto classes = CharacterClasses::getClasses();
    if (!classes.contains(className)) return 10;  // Значение по умолчанию, если класс не найден

    const auto& classInfo = classes[className];

    int minSkill = 10, maxSkill = 20;

    // Определяем диапазон навыка в зависимости от его типа
    if (skill == "Оружие") {
        minSkill = classInfo.minWeapon;
        maxSkill = classInfo.maxWeapon;
    }
    else if (skill == "Медицина") {
        minSkill = classInfo.minMedicine;
        maxSkill = classInfo.maxMedicine;
    }
    else if (skill == "Механика") {
        minSkill = classInfo.minMechanic;
        maxSkill = classInfo.maxMechanic;
    }
    else if (skill == "Электроника") {
        minSkill = classInfo.minElectronics;
        maxSkill = classInfo.maxElectronics;
    }

    // Генерируем случайное значение навыка в заданном диапазоне
    return QRandomGenerator::global()->bounded(minSkill, maxSkill + 1);
}
