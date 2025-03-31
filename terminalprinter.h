#ifndef TERMINALPRINTER_H
#define TERMINALPRINTER_H

#include <QObject>
#include <QTextEdit>
#include <QTimer>

class TerminalPrinter : public QObject
{
    Q_OBJECT

public:
    enum class Mode {
        Log,
        Journal,
        Narrative
    };

    explicit TerminalPrinter(QTextEdit* terminalOutput, QObject* parent = nullptr);
    ~TerminalPrinter();

    void setMode(Mode mode);            // смена режима
    void print(const QString& message); // универсальный метод печати

private slots:
    void printNextChar(); // слот таймера для постепенной печати

private:
    QTextEdit* terminal = nullptr;
    Mode currentMode = Mode::Log;

    // Для постепенной печати
    QTimer* charPrintTimer = nullptr;
    QString pendingMessage;
    int currentCharIndex = 0;

    void printImmediate(const QString& text);
    void printLetterByLetter(const QString& text);
    QString applyStyle(const QString& rawText);
};

#endif // TERMINALPRINTER_H
