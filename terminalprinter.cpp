#include "terminalprinter.h"
#include <QTextCursor>

TerminalPrinter::TerminalPrinter(QTextEdit* terminalOutput, QObject* parent)
    : QObject(parent),
    terminal(terminalOutput),
    currentMode(Mode::Log),
    charPrintTimer(new QTimer(this))
{
    charPrintTimer->setInterval(20); // скорость печати: 1 символ = 20 мс
    connect(charPrintTimer, &QTimer::timeout, this, &TerminalPrinter::printNextChar);
}

TerminalPrinter::~TerminalPrinter()
{
    if (charPrintTimer && charPrintTimer->isActive())
        charPrintTimer->stop();
}

void TerminalPrinter::setMode(Mode mode)
{
    currentMode = mode;
}

void TerminalPrinter::print(const QString& message)
{
    if (!terminal) return;

    if (currentMode == Mode::Log)
    {
        printImmediate(applyStyle(message));
    }
    else
    {
        printLetterByLetter(message); // без HTML-тегов
    }
}

void TerminalPrinter::printImmediate(const QString& text)
{
    terminal->append(text);
}

void TerminalPrinter::printLetterByLetter(const QString& text)
{
    if (charPrintTimer->isActive())
        charPrintTimer->stop();

    pendingMessage = text;
    currentCharIndex = 0;

    QTextCursor cursor = terminal->textCursor();
    cursor.movePosition(QTextCursor::End);
    terminal->setTextCursor(cursor);

    terminal->insertPlainText(""); // начать вывод на текущей строке
    charPrintTimer->start();
}

void TerminalPrinter::printNextChar()
{
    if (currentCharIndex >= pendingMessage.length())
    {
        terminal->append(""); // переход на следующую строку

        // Применение стиля после завершения печати (если необходимо)
        QString styledText = applyStyle(pendingMessage);

        // Удаляем текущую строку (plain) и вставляем её снова с HTML-форматированием
        terminal->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
        terminal->textCursor().removeSelectedText();
        terminal->append(styledText);

        charPrintTimer->stop();
        return;
    }

    // Добавляем следующий символ (без HTML)
    QChar ch = pendingMessage.at(currentCharIndex);
    terminal->moveCursor(QTextCursor::End);
    terminal->insertPlainText(ch);
    currentCharIndex++;
}

QString TerminalPrinter::applyStyle(const QString& rawText)
{
    switch (currentMode)
    {
    case Mode::Log:
        return QString("<span style=\"color:#7f7f7f;\">%1</span>").arg(rawText);
    case Mode::Journal:
        return QString("<span style=\"color:#bfa75a;\">%1</span>").arg(rawText);
    case Mode::Narrative:
        return QString("<span style=\"color:#df4c2e;\"><b>%1</b></span>").arg(rawText);
    default:
        return rawText;
    }
}
