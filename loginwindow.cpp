#include "loginwindow.h"
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

}

void LoginWindow::vkLogin()
{

}

void LoginWindow::quitApp()
{
    QCoreApplication::quit();
}

