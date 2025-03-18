#ifndef CHARACTER_CLASSES_H
#define CHARACTER_CLASSES_H

#include <QString>
#include <QMap>

// Класс для работы с характеристиками игровых классов
class CharacterClasses
{
public:
    // Структура для хранения диапазонов навыков класса
    struct ClassInfo {
        int minWeapon, maxWeapon;       // Навык владения оружием
        int minMedicine, maxMedicine;   // Навык медицины
        int minMechanic, maxMechanic;   // Навык механики
        int minElectronics, maxElectronics; // Навык электроники
    };

    // Возвращает карту доступных классов и их характеристик
    static QMap<QString, ClassInfo> getClasses();
};

#endif // CHARACTER_CLASSES_H
