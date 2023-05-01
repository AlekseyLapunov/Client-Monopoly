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

    bool ok = false;

    pUserMetaInfo()->get()->setHostInfo(pServer()->get()->checkIfNoNeedToAuth(ok));

    if(ok)
    {
        switchToMenuWindow();
        return;
    }
    else
    {
        qDebug().noquote() << "Need to login";
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
    setDisabled(true);
    bool ok = false;
    pUserMetaInfo()->get()->setHostInfo
    (
        flag == LoginWindow::Vk ? pServer()->get()->doVkLogin(ok)
                                : pServer()->get()->doGoogleLogin(ok)
    );
    if(!ok)
    {
        execErrorBox((QString::fromStdString(ssClassNames[ServerCommCN] +
                     (flag == LoginWindow::Vk ? ssRuntimeErrors[VkAuthFail]
                                              : ssRuntimeErrors[VkAuthFail]))),
                     this);
        setDisabled(false);
        return;
    }
    setDisabled(false);
    switchToMenuWindow();
}

void LoginWindow::switchToMenuWindow()
{
    hide();
    pMenuWindow.get()->show();
}

