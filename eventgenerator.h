#ifndef EVENTGENERATOR_H
#define EVENTGENERATOR_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QVector>

class TerminalPrinter;

class EventGenerator : public QObject
{
    Q_OBJECT

public:
    explicit EventGenerator(TerminalPrinter* terminal, QObject* parent = nullptr);
    void generate();

private:
    TerminalPrinter* terminal;

    enum class EventType {
        Negative,
        Neutral,
        Positive,
        Anomaly
    };

    struct AnomalyGrade {
        QString name;
        QString color;
    };

    // Таблицы
    QMap<EventType, QVector<QString>> eventTexts;
    QMap<int, AnomalyGrade> anomalyGrades;

    void loadEventTemplates();
    void loadAnomalyGrades();

    bool isAnomaly(int roll) const;
    QString describeAnomaly(int roll) const;
    EventType categorize(int roll) const;
};

#endif // EVENTGENERATOR_H
