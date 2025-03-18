#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // Инициализация Qt-приложения
    MainWindow w; // Создание главного окна
    w.show(); // Отображение окна
    return a.exec(); // Запуск цикла обработки событий
}
