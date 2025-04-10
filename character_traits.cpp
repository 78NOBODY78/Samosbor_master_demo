#include "character_traits.h"

// Возвращает карту особенностей персонажа и их описаний
QMap<QString, QString> CharacterTraits::getTraits()
{
    return {
        {"Выносливый", "Обладает повышенной физической выносливостью."},
        {"Быстрый", "Быстрее реагирует и передвигается."},
        {"Лидер", "Умеет вести за собой."},
        {"Тактик", "Хорошо просчитывает ситуацию."},
        {"Интуитивный", "Часто предугадывает события."},
        {"Спокойный", "Сохраняет хладнокровие в сложных ситуациях."},
        {"Смекалистый", "Быстро находит выход из сложных ситуаций."},
        {"Харизматичный", "Легко находит общий язык с другими."},
        {"Бесстрашный", "Не поддается страху."},
        {"Тихий", "Умеет бесшумно передвигаться."},
        {"Рассудительный", "Принимает взвешенные решения."},
        {"Жесткий", "Не щадит противников."},
        {"Эрудит", "Обладает глубокими знаниями."}
    };
}
