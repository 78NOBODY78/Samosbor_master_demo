#include "mastercontrolwindow.h"
#include "ui_mastercontrolwindow.h"

#include "samosborcontroller.h"
#include "terminalprinter.h"
#include "eventgenerator.h"

MasterControlWindow::MasterControlWindow(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::MasterControlWindow),
    samosbor(nullptr),
    terminal(nullptr),
    eventGenerator(nullptr)
{
    qDebug() << "MasterControlWindow создаётся!";
    ui->setupUi(this);
    setupControllers();
    setupConnections();
}

MasterControlWindow::~MasterControlWindow()
{
    delete ui;
}

// === Кнопка Назад ===
void MasterControlWindow::on_pushButtonBack_clicked() {
    if (parentWidget()) {
        parentWidget()->show();  // Возвращаемся к GameModeWindow
    }
    this->close();
}

// === Инициализация контроллеров ===
void MasterControlWindow::setupControllers()
{
    qDebug() << "setupControllers стартует";
    terminal = new TerminalPrinter(ui->textEditTerminal, this);
    samosbor = new SamosborController(this);
    eventGenerator = new EventGenerator(terminal, this);
    connect(samosbor, &SamosborController::logMessage, terminal, &TerminalPrinter::print);
    qDebug() << "setupControllers завершён";
}


// === Подключение интерфейсных элементов к логике ===
void MasterControlWindow::setupConnections()
{
    // Переключатели режима терминала
    qDebug() << "setupConnections стартует";
    connect(ui->radioButtonLogMode, &QRadioButton::toggled, this, [this](bool checked){
        if (checked && terminal)
            terminal->setMode(TerminalPrinter::Mode::Log);
    });

    connect(ui->radioButtonJournalMode, &QRadioButton::toggled, this, [this](bool checked){
        if (checked && terminal)
            terminal->setMode(TerminalPrinter::Mode::Journal);
    });

    connect(ui->radioButtonNarrativeMode, &QRadioButton::toggled, this, [this](bool checked){
        if (checked && terminal)
            terminal->setMode(TerminalPrinter::Mode::Narrative);
    });

    // Самосбор: подключение кнопок к слотам
    connect(ui->pushButtonSamosborStart, &QPushButton::clicked,
            this, &MasterControlWindow::on_pushButtonSamosborStart_clicked);
    connect(ui->pushButtonSamosborPause, &QPushButton::clicked,
            this, &MasterControlWindow::on_pushButtonSamosborPause_clicked);
    connect(ui->pushButtonSamosborResume, &QPushButton::clicked,
            this, &MasterControlWindow::on_pushButtonSamosborResume_clicked);
    connect(ui->pushButtonSamosborManualTrigger, &QPushButton::clicked,
            this, &MasterControlWindow::on_pushButtonSamosborManualTrigger_clicked);
    connect(ui->pushButtonSamosborManualSlowdown, &QPushButton::clicked,
            this, &MasterControlWindow::on_pushButtonSamosborManualSlowdown_clicked);

    // Кнопка включения/выключения терминала
    connect(ui->pushButtonTerminalON_OFF, &QPushButton::clicked,
            this, &MasterControlWindow::on_pushButtonTerminalON_OFF_clicked);

    // Случайное событие
    connect(ui->pushButtonRandomEvent, &QPushButton::clicked,
            this, &MasterControlWindow::on_pushButtonRandomEvent_clicked);
    qDebug() << "setupConnections завершён";
}

// === Слоты управления терминалом ===
void MasterControlWindow::on_radioButtonLogMode_toggled(bool checked) {
    if (checked && terminal)
        terminal->setMode(TerminalPrinter::Mode::Log);
}

void MasterControlWindow::on_radioButtonJournalMode_toggled(bool checked) {
    if (checked && terminal)
        terminal->setMode(TerminalPrinter::Mode::Journal);
}

void MasterControlWindow::on_radioButtonNarrativeMode_toggled(bool checked) {
    if (checked && terminal)
        terminal->setMode(TerminalPrinter::Mode::Narrative);
}

void MasterControlWindow::on_pushButtonTerminalON_OFF_clicked() {
    bool visible = ui->textEditTerminal->isVisible();
    ui->textEditTerminal->setVisible(!visible);

    ui->pushButtonTerminalON_OFF->setText(visible ? "Вкл" : "Выкл");

    if (terminal)
        terminal->print(visible ? "[Терминал выключен]" : "[Терминал включен]");
}

// === Слоты управления самосбором ===
void MasterControlWindow::on_pushButtonSamosborStart_clicked() {
    if (samosbor)
        samosbor->start();
}

void MasterControlWindow::on_pushButtonSamosborPause_clicked() {
    if (samosbor)
        samosbor->pause();
}

void MasterControlWindow::on_pushButtonSamosborResume_clicked() {
    if (samosbor)
        samosbor->resume();
}

void MasterControlWindow::on_pushButtonSamosborManualTrigger_clicked() {
    if (samosbor)
        samosbor->manualTrigger();
}

void MasterControlWindow::on_pushButtonSamosborManualSlowdown_clicked() {
    if (samosbor)
        samosbor->manualSlowdown();
}

// === Случайное событие ===
void MasterControlWindow::on_pushButtonRandomEvent_clicked() {
    if (eventGenerator)
        eventGenerator->generate();
}
