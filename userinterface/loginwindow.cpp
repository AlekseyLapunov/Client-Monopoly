#include "userinterface/loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                         unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                         QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    if(!*newServerPtr || !*newMetaInfoPtr)
        throw std::runtime_error(ssClassNames[LoginWindowCN] + ssErrorsContent[PtrLinkFail]);

    setupPointers(*newServerPtr, *newMetaInfoPtr);

    pMenuWindow = unique_ptr<MenuWindow>(new MenuWindow(pServer(), pUserMetaInfo()));

    connect(pMenuWindow.get(), &MenuWindow::goToLoginWindow,
            this, &LoginWindow::show);
    connect(pMenuWindow.get(), &MenuWindow::goToLoginWindow,
            this, [=](){ dontCheckIfAuthorized = true; });
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::show()
{
#ifdef DEBUG_SKIP_LOGIN
    switchToMenuWindow();
    return;
#endif
    ui->setupUi(this);

    bool ok = false;
    if(!dontCheckIfAuthorized)
        pUserMetaInfo()->get()->setHostInfo(pServer()->get()->checkIfNoNeedToAuth(ok));
    else dontCheckIfAuthorized = false;

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
    baseLogin(LoginType::Google);
}

void LoginWindow::vkLogin()
{
    baseLogin(LoginType::Vk);
}

void LoginWindow::guestLogin()
{
    baseLogin(LoginType::Guest);
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
        (flag == LoginType::Vk)     ? pServer()->get()->doVkLogin(ok)     :
        (flag == LoginType::Google) ? pServer()->get()->doGoogleLogin(ok) :
                                      pServer()->get()->doGuestLogin(ok)
    );
    if(!ok)
    {
        execErrorBox
        (
            (QString::fromStdString(ssClassNames[ServerCommCN] +
            ((flag == LoginType::Vk)     ? ssErrorsContent[VkAuthFail]     :
             (flag == LoginType::Google) ? ssErrorsContent[GoogleAuthFail] :
                                           ssErrorsContent[GuestAuthFail]))
        ), this);
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

