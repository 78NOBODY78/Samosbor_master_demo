#ifndef SAMOSBORCONTROLLER_H
#define SAMOSBORCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QString>
#include <QDateTime>

class SamosborController : public QObject
{
    Q_OBJECT

public:
    explicit SamosborController(QObject* parent = nullptr);
    ~SamosborController();
    // Управление
    void start();
    void pause();
    void resume();
    void manualTrigger();
    void manualSlowdown();

    // Получение информации для UI
    QString getElapsedTimeString() const;
    QString getRangeInfo() const;
    QString getState() const;

    // Сброс таймера и диапазона
    void reset();

signals:
    void logMessage(const QString& message);         // для терминала
    void rangeChanged(const QString& rangeInfo);     // для label
    void timeUpdated(const QString& time);           // для label
    void stateChanged(const QString& state);         // для label

private slots:
    void update();               // вызывается каждые 5 секунд
    void handleRangeShrink();    // вызывается каждые 2 минуты

private:
    enum class State {
        Idle,
        Running,
        Paused,
        Slowdown
    };

    // Вспомогательные методы
    void generateRandomNumber();        // Основная генерация
    void triggerSamosbor();             // Обработка самосбора
    void triggerSlowdown();             // Обработка замедления

    // Состояние
    State currentState = State::Idle;

    // Таймеры
    QTimer* mainTimer = nullptr;
    QTimer* rangeShrinkTimer = nullptr;
    QTime startTime = QTime(0,0);

    // Диапазон
    int currentMinRange = initialMinRange;
    int currentMaxRange = initialMaxRange;

    // Защита от повторного самосбора
    QDateTime lastSamosborTime = QDateTime();

    // Константы (параметры управления)
    static constexpr int cooldownSeconds = 120;
    static constexpr int initialMinRange = -100;
    static constexpr int initialMaxRange = 100;
};

#endif // SAMOSBORCONTROLLER_H
