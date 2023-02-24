#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "helpers/basewin.h"
#include "menuwindow.h"

#include <QWidget>
#include <QCoreApplication>

namespace Ui { class LoginWindow; }

class LoginWindow : public QWidget, public BaseWin
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void googleLogin();
    void vkLogin();
    void quitApp();

private:
    void switchToMenuWindow();

private:
    Ui::LoginWindow *ui;
    unique_ptr<MenuWindow> menuWindow {new MenuWindow()};

};
#endif // LOGINWINDOW_H
