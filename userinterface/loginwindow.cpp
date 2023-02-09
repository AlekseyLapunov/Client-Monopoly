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

void LoginWindow::sharePointers(ServerCommunicator *pServer, UserMetaInfo *pUser)
{
    this->pServer = pServer;
    this->pUser = pUser;
}

void LoginWindow::googleLogin()
{
    if(!pServer->doGoogleLogin())
    {
        // Error logining through Google
    }
    else
    {
        openMenuWindow();
    }
}

void LoginWindow::vkLogin()
{
    if(!pServer->doVkLogin())
    {
        // Error logining through Vk
    }
    else
    {
        openMenuWindow();
    }
}

void LoginWindow::quitApp()
{
    QCoreApplication::quit();
}

void LoginWindow::openMenuWindow()
{
    //this->close();
}

