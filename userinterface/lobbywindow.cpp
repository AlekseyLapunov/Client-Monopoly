#include "lobbywindow.h"
#include "ui_lobbywindow.h"

LobbyWindow::LobbyWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                         unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                         QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LobbyWindow)
{
    if(!*newServerPtr || !*newMetaInfoPtr)
        throw std::runtime_error(loginWindowClassName + errorPtrLink);

    setupPointers(*newServerPtr, *newMetaInfoPtr);

    ui->setupUi(this);
}

LobbyWindow::~LobbyWindow()
{
    delete ui;
}

void LobbyWindow::giveFirstContext(LobbyFullInfo &context)
{
    m_context = context;
}

void LobbyWindow::windowDataRefresh()
{
    this->setDisabled(true);
    setUpByPrivileges(this->pUserMetaInfo()->get()->getHostInfo().uniqueUserId);
    setUpLobbySystem(m_context.lobbySystem);
    setUpGameSettings(m_context.gameSettings);
    setUpUsersInTable(m_context.usersInTable);
    this->setEnabled(true);
}

void LobbyWindow::toggleLobbyVision()
{

}

void LobbyWindow::startGame()
{

}

void LobbyWindow::setLobbySettings()
{

}

void LobbyWindow::setGameSettings()
{

}

void LobbyWindow::leaveLobby()
{
    if(makeDialog(BaseWin::leaveLobby) == 0)
    {
        this->hide();
        emit goToMenuWindow();
    }
}

void LobbyWindow::toggleReadyStatus()
{

}

void LobbyWindow::quitApp()
{
    QCoreApplication::quit();
}

void LobbyWindow::setUpByPrivileges(int uniqueHostId)
{

}

void LobbyWindow::setUpLobbySystem(LobbySystemInfo& lsiContext)
{

}

void LobbyWindow::setUpGameSettings(GameSettingsInfo& gsContext)
{

}

void LobbyWindow::setUpUsersInTable(std::vector<UserShortInfo>& usiContextVec)
{

}
