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
    HostUserData hostUser = pUserMetaInfo()->get()->getHostInfo();
    context.usersInTable.push_back({hostUser.userName, hostUser.userRpCount, false});
    m_context = context;
}

void LobbyWindow::windowDataRefresh()
{
    ui->setupUi(this);
    this->setDisabled(true);
    setUpLobbySystem(m_context.lobbySystem);
    setUpGameSettings(m_context.gameSettings);
    setUpUsersInTable(m_context.usersInTable);
    setUpByPrivilege();
    this->setEnabled(true);
}

void LobbyWindow::definePrivilege()
{
    int uniqueHostId = this->pUserMetaInfo()->get()->getHostInfo().uniqueUserId;
    // If lobby is ranked
    if(m_context.lobbySystem.uniqueId < 0)
        m_privilegeType = rankedGuest;
    else
    // If host user is the lobby host
    if(m_context.lobbySystem.ownerUniqueId == uniqueHostId)
    {
        m_privilegeType = owner;
    }
    else
        m_privilegeType = guest;
}

void LobbyWindow::setUpByPrivilege()
{
    definePrivilege();
    switch (m_privilegeType)
    {
    case rankedGuest:
        setButtonsVisibility(false);
        this->ui->lLobbyName->setText(rankedLobbyText);
        this->ui->lPasswordConst->setVisible(false);
        this->ui->lePassword->setVisible(false);
        this->ui->lLobbyNameConst->setVisible(false);
        this->ui->leLobbyName->setVisible(false);
        this->ui->bToggleReady->setVisible(false);
        break;
    case guest:
        setButtonsVisibility(false);
        break;
    case owner:
        setButtonsVisibility(true);
        this->ui->bLeaveLobby->setText(deleteLobbyText);
        this->ui->aLeaveLobby->setText(deleteLobbyText);
        break;
    default:
        break;
    }
}

void LobbyWindow::setButtonsVisibility(bool areVisible)
{
    this->ui->bSetGameSettings->setVisible(areVisible);
    this->ui->bSetLobbySettings->setVisible(areVisible);
    this->ui->bStartGame->setVisible(areVisible);
    this->ui->bToggleLobbyVision->setVisible(areVisible);

    // These should be always visible by default
    this->ui->bToggleReady->setVisible(true);
    this->ui->bLeaveLobby->setVisible(true);
}

void LobbyWindow::setUpLobbySystem(LobbySystemInfo& lsiContext)
{
    this->ui->lLobbyName->setText(lsiContext.lobbyName);
    this->ui->leLobbyName->setText(lsiContext.lobbyName);
    this->ui->leLobbyName->setReadOnly(true);
    if(lsiContext.isPassworded == true)
    {
        this->ui->lePassword->setText(passwordPlaceholder);
        this->ui->lePassword->setReadOnly(true);
    }
    this->ui->sbMaxPlayers->setValue(lsiContext.maxPlayersCount);
    this->ui->sbMaxPlayers->setReadOnly(true);
}

void LobbyWindow::setUpGameSettings(GameSettingsInfo& gsContext)
{
    this->ui->sbTurnTime->setValue(gsContext.turnTime);
    this->ui->sbTurnTime->setReadOnly(true);
    this->ui->sbMaxTurns->setValue(gsContext.turnsRightBorder);
    this->ui->sbMaxTurns->setReadOnly(true);
    this->ui->dsbMaxBalance->setValue(gsContext.moneyRightBorder);
    this->ui->dsbMaxBalance->setReadOnly(true);
    this->ui->chbIsTurnsInfinite->setCheckable(false);
    this->ui->chbIsBalanceInfinite->setCheckable(false);
}

void LobbyWindow::setUpUsersInTable(std::vector<UserShortInfo>& usiContextVec)
{

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
    // Make delete lobby request if host user leaves
    if(m_privilegeType == owner)
        pServer()->get()->deleteLobbyRequest(m_context.lobbySystem.uniqueId);
}

void LobbyWindow::toggleReadyStatus()
{

}

void LobbyWindow::quitApp()
{
    if(makeDialog(BaseWin::quitApp) == 0)
    {
        // Make delete lobby request if host user leaves
        if(m_privilegeType == owner)
            pServer()->get()->deleteLobbyRequest(m_context.lobbySystem.uniqueId);
        QCoreApplication::quit();
    }
}
