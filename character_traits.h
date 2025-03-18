#ifndef CHARACTER_TRAITS_H
#define CHARACTER_TRAITS_H

#include <QString>
#include <QMap>

// Класс для работы с особенностями персонажа
class CharacterTraits
{
public:
    static QMap<QString, QString> getTraits();  // Возвращает карту особенностей и их описаний
};

#endif // CHARACTER_TRAITS_H
