#include "eventgenerator.h"
#include "terminalprinter.h"

#include <QRandomGenerator>

EventGenerator::EventGenerator(TerminalPrinter* terminal, QObject* parent)
    : QObject(parent), terminal(terminal)
{
    loadEventTemplates();
    loadAnomalyGrades();
}

void EventGenerator::generate()
{
    int roll = QRandomGenerator::global()->bounded(1, 101);

    if (isAnomaly(roll)) {
        QString effect = describeAnomaly(roll);
        terminal->print(QString("🧨 АНОМАЛИЯ [%1]: %2").arg(roll).arg(effect));
        return;
    }

    EventType type = categorize(roll);
    auto texts = eventTexts.value(type);
    QString event = texts.isEmpty() ? "Неизвестное событие..." : texts.at(QRandomGenerator::global()->bounded(texts.size()));

    QString typeLabel;
    switch (type) {
    case EventType::Negative: typeLabel = "Негативное событие"; break;
    case EventType::Neutral:  typeLabel = "Фоновое событие"; break;
    case EventType::Positive: typeLabel = "Позитивное событие"; break;
    default: break;
    }

    terminal->print(QString("🎲 [%1] %2").arg(roll).arg(typeLabel));
    terminal->print("📍 " + event);
}

bool EventGenerator::isAnomaly(int roll) const
{
    static QVector<int> excluded = {2, 3, 5, 23, 31, 41, 61, 71, 99, 100};
    if (excluded.contains(roll)) return false;
    if (roll == 13 || roll == 66) return true;

    // Проверка на простое число
    if (roll < 2) return false;
    for (int i = 2; i * i <= roll; ++i)
        if (roll % i == 0) return false;

    return true;
}

EventGenerator::EventType EventGenerator::categorize(int roll) const
{
    if (roll <= 20) return EventType::Negative;
    if (roll <= 80) return EventType::Neutral;
    return EventType::Positive;
}

QString EventGenerator::describeAnomaly(int roll) const
{
    if (anomalyGrades.contains(roll)) {
        auto g = anomalyGrades.value(roll);
        return QString("<span style='color:%1;'><b>%2</b></span>").arg(g.color, g.name);
    }
    return "<span style='color:#ff0000;'>НЕОПОЗНАННОЕ АНОМАЛЬНОЕ ЯВЛЕНИЕ</span>";
}

void EventGenerator::loadEventTemplates()
{
    eventTexts[EventType::Negative] = {
        "Устройство издаёт тревожный писк и замолкает.",
        "Связь с внешним периметром потеряна.",
        "Температура резко падает — что-то случилось с климат-контролем."
    };

    eventTexts[EventType::Neutral] = {
        "Система провела самотестирование. Всё в норме.",
        "Пыль медленно оседает в коридоре.",
        "Проходит очередной цикл работы освещения."
    };

    eventTexts[EventType::Positive] = {
        "Найден источник питания. Возможен перезапуск.",
        "Случайная проверка выявила полезные данные.",
        "Запас провизии продержит группу ещё на сутки."
    };
}

void EventGenerator::loadAnomalyGrades()
{
    anomalyGrades = {
        {7,  {"Моментальное озарение", "#00ff00"}},
        {11, {"Подключение к неизвестному каналу", "#00aa00"}},
        {17, {"Благословение поля", "#33cc66"}},
        {19, {"Отклонение реальности", "#cccc00"}},
        {29, {"Эхо чужого сигнала", "#999933"}},
        {37, {"Память системы повреждена", "#ff9900"}},
        {47, {"Энергетический сбой", "#cc6600"}},
        {53, {"Разлом в пространстве", "#cc3300"}},
        {66, {"Активация протокола ИЗОЛЯЦИЯ", "#ff0000"}},
        {79, {"Поле Хаоса: воспоминания не совпадают", "#990000"}},
        {83, {"Пси-контакт. Группа не осознаёт происходящего.", "#990033"}},
        {97, {"ПОЛНАЯ ДЕГРАДАЦИЯ КОНТУРА", "#660000"}},
        {13, {"Аномалия 13. Не трогать. Никогда.", "#8b0000"}}
    };
}
