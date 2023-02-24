#include "userinterface/loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::googleLogin()
{
    if(!pServer->get()->doGoogleLogin())
    {
        // Error logining through Google
    }
    else
    {
        switchToMenuWindow();
    }
}

void LoginWindow::vkLogin()
{
    if(!pServer->get()->doVkLogin())
    {
        // Error logining through Vk
    }
    else
    {
        switchToMenuWindow();
    }
}

void LoginWindow::quitApp()
{
    QCoreApplication::quit();
}

void LoginWindow::switchToMenuWindow()
{
    this->hide();
    menuWindow->show();
}

