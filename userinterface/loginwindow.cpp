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

    pMenuWindow = unique_ptr<MenuWindow>(new MenuWindow(pServer(), pUserMetaInfo()/*, this*/));

    connect(pMenuWindow.get(), &MenuWindow::switchToLoginWindow,
            this, &LoginWindow::show);
    connect(pMenuWindow.get(), &MenuWindow::switchToLoginWindow,
            this, [=](){ dontCheckIfAuthorized = true; });

    ui->setupUi(this);
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
    setDisabled(true);

    ResponseFromServerComm<HostUserData> response;

    if(!dontCheckIfAuthorized)
    {
        dontCheckIfAuthorized = true;
        response = pServer()->get()->checkIfNoNeedToAuth();

        if(response.responseFlag == AllGoodRf)
        {
            pUserMetaInfo()->get()->setHostInfo(response.payload);
            switchToMenuWindow();
            return;
        }
    }

    qDebug().noquote() << QString("%1Need to login")
                          .arg(QString::fromStdString(ssClassNames[LoginWindowCN]));
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

void LoginWindow::baseLogin(uint8_t loginFlag)
{
    setDisabled(true);

    ResponseFromServerComm<HostUserData> response;

    switch (loginFlag)
    {
    case LoginType::Google:
        response = pServer()->get()->doGoogleLogin();
        break;
    case LoginType::Vk:
        response = pServer()->get()->doVkLogin();
        break;
    case LoginType::Guest:
        response = pServer()->get()->doGuestLogin();
        break;
    default:
        return;
    }

    if(response.responseFlag != AllGoodRf)
    {
        execErrorBox
        (
            (QString::fromStdString(ssClassNames[ServerCommCN] +
            ((loginFlag == LoginType::Vk)     ? ssErrorsContent[VkAuthFail]     :
             (loginFlag == LoginType::Google) ? ssErrorsContent[GoogleAuthFail] :
                                                ssErrorsContent[GuestAuthFail]))
        ), this);
        setDisabled(false);
        return;
    }

    pUserMetaInfo()->get()->setHostInfo(response.payload);

    setDisabled(false);
    switchToMenuWindow();
}

void LoginWindow::switchToMenuWindow()
{
    setDisabled(true);
    pMenuWindow.get()->needToCheckActiveGame = true;
    pMenuWindow.get()->show();
    hide();
}

void LoginWindow::checkTimedOutCounter()
{

}

