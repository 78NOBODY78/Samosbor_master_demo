#ifndef MASTERCONTROLWINDOW_H
#define MASTERCONTROLWINDOW_H

#include <QWidget>

class SamosborController;
class TerminalPrinter;
class EventGenerator;

QT_BEGIN_NAMESPACE
namespace Ui { class MasterControlWindow; }
QT_END_NAMESPACE

class MasterControlWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MasterControlWindow(QWidget *parent = nullptr);
    ~MasterControlWindow();

private:
    Ui::MasterControlWindow *ui;

    // Указатели на контроллеры
    SamosborController* samosbor;
    TerminalPrinter* terminal;
    EventGenerator* eventGenerator;

    // Инициализация внутренних компонентов
    void setupConnections();
    void setupControllers();

private slots:
    // Терминал
    void on_radioButtonLogMode_toggled(bool checked);
    void on_radioButtonJournalMode_toggled(bool checked);
    void on_radioButtonNarrativeMode_toggled(bool checked);
    void on_pushButtonTerminalON_OFF_clicked();

    // Самосбор
    void on_pushButtonSamosborStart_clicked();
    void on_pushButtonSamosborPause_clicked();
    void on_pushButtonSamosborResume_clicked();
    void on_pushButtonSamosborManualTrigger_clicked();
    void on_pushButtonSamosborManualSlowdown_clicked();

    // События
    void on_pushButtonRandomEvent_clicked();

    // Назад
    void on_pushButtonBack_clicked();
};

#endif // MASTERCONTROLWINDOW_H
