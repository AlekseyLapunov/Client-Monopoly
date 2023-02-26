#include "userinterface/loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                         unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                         QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    if(!*newServerPtr || !*newMetaInfoPtr)
        throw std::runtime_error("LoginWindow: helpers pointers are nullptr");

    setupPointers(*newServerPtr, *newMetaInfoPtr);

    ui->setupUi(this);

    pMenuWindow = unique_ptr<MenuWindow>(new MenuWindow(pServer(), pUserMetaInfo()));

    connect(pMenuWindow.get(), &MenuWindow::goToLoginWindow,
            this, &LoginWindow::show);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::googleLogin()
{
    baseLogin(LoginWindow::google);
}

void LoginWindow::vkLogin()
{
    baseLogin(LoginWindow::vk);
}

void LoginWindow::quitApp()
{
    QCoreApplication::quit();
}

void LoginWindow::baseLogin(serviceFlag flag)
{
    try
    {
        pUserMetaInfo()->get()->setHostInfo
            (
                    flag == LoginWindow::google ? pServer()->get()->doGoogleLogin()
                                                : pServer()->get()->doVkLogin()
            );
    }
    catch(const std::exception &e)
    {
        QMessageBox::critical(this, this->windowTitle(), tr("Не удалось войти: %1").arg(e.what()));
        return;
    }
    switchToMenuWindow();
}

void LoginWindow::switchToMenuWindow()
{
    this->hide();
    pMenuWindow.get()->windowDataRefresh();
    pMenuWindow.get()->show();
}

