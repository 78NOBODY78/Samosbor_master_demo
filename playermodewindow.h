#ifndef PLAYERMODEWINDOW_H
#define PLAYERMODEWINDOW_H

#include <QWidget>
#include <QPointer>  // ✅ Безопасный указатель
#include "createcharacterwindow.h"

namespace Ui {
class PlayerModeWindow;
}

class PlayerModeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerModeWindow(QWidget *parent = nullptr);
    ~PlayerModeWindow();

private:
    Ui::PlayerModeWindow *ui;
    QPointer<CreateCharacterWindow> characterWindow;  // ✅ Используем QPointer

private slots:
    void onCreateCharacterClicked();
    void onSelectCharacterClicked();
    void onBackClicked();
};

#endif // PLAYERMODEWINDOW_H
