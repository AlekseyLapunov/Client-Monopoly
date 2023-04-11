#include "userinterface/loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                         unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                         QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    if(!*newServerPtr || !*newMetaInfoPtr)
        throw std::runtime_error(ssLoginWindowClassName + ssRuntimeErrors[PtrLinkFail]);

    setupPointers(*newServerPtr, *newMetaInfoPtr);

    ui->setupUi(this);

    pMenuWindow = unique_ptr<MenuWindow>(new MenuWindow(pServer(), pUserMetaInfo(), this));

    connect(pMenuWindow.get(), &MenuWindow::goToLoginWindow,
            this, &LoginWindow::show);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::googleLogin()
{
    baseLogin(LoginWindow::Google);
}

void LoginWindow::vkLogin()
{
    baseLogin(LoginWindow::Vk);
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
                flag == LoginWindow::Google ? pServer()->get()->doGoogleLogin()
                                            : pServer()->get()->doVkLogin()
            );
    }
    catch(const std::exception &e)
    {
        execErrorBox(e.what(), this);
        return;
    }
    switchToMenuWindow();
}

void LoginWindow::switchToMenuWindow()
{
    hide();
    pMenuWindow.get()->showAndRefresh();
}

