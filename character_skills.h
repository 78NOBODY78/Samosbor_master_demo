#ifndef CHARACTER_SKILLS_H
#define CHARACTER_SKILLS_H

#include <QString>
#include "character_classes.h"

// Класс для работы с навыками персонажа
class CharacterSkills
{
public:
    // Генерирует значение навыка в зависимости от выбранного класса
    static int generateSkill(const QString& className, const QString& skill);
};

#endif // CHARACTER_SKILLS_H
