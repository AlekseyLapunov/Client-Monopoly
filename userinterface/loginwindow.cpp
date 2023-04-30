#include "userinterface/loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                         unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                         QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    if(!*newServerPtr || !*newMetaInfoPtr)
        throw std::runtime_error(ssClassNames[LoginWindowCN] + ssRuntimeErrors[PtrLinkFail]);

    setupPointers(*newServerPtr, *newMetaInfoPtr);

    pMenuWindow = unique_ptr<MenuWindow>(new MenuWindow(pServer(), pUserMetaInfo()));

    connect(pMenuWindow.get(), &MenuWindow::goToLoginWindow,
            this, &LoginWindow::show);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::show()
{
#ifdef DEBUG
    switchToMenuWindow();
    return;
#endif
    ui->setupUi(this);

    try
    {
        pUserMetaInfo()->get()->setHostInfo(pServer()->get()->tryIfNoNeedToAuth());
    }
    catch (std::exception &e)
    {
        qDebug().noquote() << QString::fromStdString(ssClassNames[LoginWindowCN]) << "Need to login.";
    }

    setDisabled(false);
    QWidget::show();
}

void LoginWindow::hide()
{
    setDisabled(true);
    QWidget::hide();
}

void LoginWindow::quitAppDialog()
{
    if(makeDialog(BaseWin::QuitApp, "", this) == 0)
        QCoreApplication::quit();
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

void LoginWindow::closeEvent(QCloseEvent *event)
{
    quitAppDialog();
    event->ignore();
}

void LoginWindow::baseLogin(short flag)
{
    try
    {
        pUserMetaInfo()->get()->setHostInfo
            (
                flag == LoginWindow::Vk ? pServer()->get()->tryVkLogin()
                                        : pServer()->get()->tryGoogleLogin()
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
    pMenuWindow.get()->show();
}

