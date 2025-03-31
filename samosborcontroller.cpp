#include "samosborcontroller.h"
#include <QRandomGenerator>
#include <QDebug>

SamosborController::SamosborController(QObject *parent)
    : QObject(parent),
    mainTimer(new QTimer(this)),
    rangeShrinkTimer(new QTimer(this)),
    currentState(State::Idle),
    currentMinRange(initialMinRange),
    currentMaxRange(initialMaxRange),
    startTime(QTime(0,0)),
    lastSamosborTime(QDateTime::currentDateTime().addSecs(-cooldownSeconds))
{
    connect(mainTimer, &QTimer::timeout, this, &SamosborController::update);
    connect(rangeShrinkTimer, &QTimer::timeout, this, &SamosborController::handleRangeShrink);
}

SamosborController::~SamosborController()
{
    mainTimer->stop();
    rangeShrinkTimer->stop();
}

void SamosborController::start()
{
    if (currentState == State::Running) return;

    startTime = QTime::currentTime();
    currentState = State::Running;
    emit stateChanged("Идёт");

    mainTimer->start(5000);
    rangeShrinkTimer->start(120000);

    emit logMessage("▶ Самосбор запущен. Диапазон: -100 / 100");
    emit rangeChanged(getRangeInfo());
    emit timeUpdated(getElapsedTimeString());
}

void SamosborController::pause()
{
    if (currentState != State::Running) return;

    mainTimer->stop();
    rangeShrinkTimer->stop();
    currentState = State::Paused;
    emit stateChanged("Пауза");

    emit logMessage("⏸ Самосбор приостановлен.");
}

void SamosborController::resume()
{
    if (currentState != State::Paused) return;

    mainTimer->start(5000);
    rangeShrinkTimer->start(120000);
    currentState = State::Running;
    emit stateChanged("Идёт");

    emit logMessage("▶ Самосбор возобновлён.");
}

void SamosborController::manualTrigger()
{
    triggerSamosbor();
}

void SamosborController::manualSlowdown()
{
    triggerSlowdown();
}

void SamosborController::update()
{
    emit timeUpdated(getElapsedTimeString());
    generateRandomNumber();
}

void SamosborController::generateRandomNumber()
{
    int value = QRandomGenerator::global()->bounded(currentMinRange, currentMaxRange + 1);
    emit logMessage(QString("🎲 Сгенерировано число: %1").arg(value));

    if (value == currentMaxRange)
    {
        if (lastSamosborTime.secsTo(QDateTime::currentDateTime()) < cooldownSeconds) {
            emit logMessage("🛑 Самосбор заблокирован (перезарядка).");
            return;
        }
        triggerSamosbor();
    }
    else if (value == currentMinRange)
    {
        triggerSlowdown();
    }
}

void SamosborController::triggerSamosbor()
{
    lastSamosborTime = QDateTime::currentDateTime();
    currentMinRange = initialMinRange;
    currentMaxRange = initialMaxRange;

    emit rangeChanged(getRangeInfo());
    emit logMessage("☢ САМОСБОР! Диапазон сброшен.");
    emit stateChanged("САМОСБОР");
}

void SamosborController::triggerSlowdown()
{
    currentState = State::Slowdown;
    emit stateChanged("Замедление");
    emit logMessage("⚠ Замедление: нижняя граница достигнута.");
}

void SamosborController::handleRangeShrink()
{
    if (currentMinRange < 0) currentMinRange += 10;
    if (currentMaxRange > 0) currentMaxRange -= 10;

    emit rangeChanged(getRangeInfo());
    emit logMessage(QString("📉 Диапазон сузился: %1").arg(getRangeInfo()));
}

QString SamosborController::getElapsedTimeString() const
{
    int seconds = startTime.secsTo(QTime::currentTime());
    return QString("%1:%2")
        .arg(seconds / 60, 2, 10, QChar('0'))
        .arg(seconds % 60, 2, 10, QChar('0'));
}

QString SamosborController::getRangeInfo() const
{
    return QString("%1 / %2").arg(currentMinRange).arg(currentMaxRange);
}

QString SamosborController::getState() const
{
    switch (currentState)
    {
    case State::Idle:      return "Ожидание";
    case State::Running:   return "Идёт";
    case State::Paused:    return "Пауза";
    case State::Slowdown:  return "Замедление";
    }
    return "Неизвестно";
}

void SamosborController::reset()
{
    mainTimer->stop();
    rangeShrinkTimer->stop();

    currentMinRange = initialMinRange;
    currentMaxRange = initialMaxRange;
    currentState = State::Idle;

    emit rangeChanged(getRangeInfo());
    emit stateChanged("Ожидание");
    emit logMessage("🔁 Самосбор сброшен.");
}
