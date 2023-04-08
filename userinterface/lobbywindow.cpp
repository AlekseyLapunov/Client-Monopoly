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
    setUpUsersInTable(*this->ui->tUsers, m_context.usersInTable);
    setUpByPrivilege();
    this->setEnabled(true);
}

void LobbyWindow::definePrivilege()
{
    int uniqueHostId = this->pUserMetaInfo()->get()->getHostInfo().uniqueUserId;
    // If lobby is ranked
    if(m_context.lobbySystem.uniqueId < 0)
        m_privilegeType = RankedGuest;
    else
    // If host user is the lobby host
    if(m_context.lobbySystem.ownerUniqueId == uniqueHostId)
    {
        m_privilegeType = Owner;
    }
    else
        m_privilegeType = Guest;
}

void LobbyWindow::setUpByPrivilege()
{
    definePrivilege();
    switch (m_privilegeType)
    {
    case RankedGuest:
        setButtonsVisibility(false);
        this->ui->lLobbyName->setText(rankedLobbyText);
        this->ui->lPasswordConst->setVisible(false);
        this->ui->lePassword->setVisible(false);
        this->ui->lLobbyNameConst->setVisible(false);
        this->ui->leLobbyName->setVisible(false);
        this->ui->bToggleReady->setVisible(false);
        break;
    case Guest:
        setButtonsVisibility(false);
        break;
    case Owner:
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
    this->ui->bApplySettings->setVisible(areVisible);
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

    this->ui->chbIsTurnsInfinite->setChecked(gsContext.isMaxTurnsInfinite);
    this->ui->sbTurnTime->setDisabled(gsContext.isMaxTurnsInfinite);

    this->ui->chbIsBalanceInfinite->setChecked(gsContext.isMaxMoneyInfinite);
    this->ui->dsbMaxBalance->setDisabled(gsContext.isMaxMoneyInfinite);

    this->ui->chbIsTurnsInfinite->setDisabled(true);
    this->ui->chbIsBalanceInfinite->setDisabled(true);
}

void LobbyWindow::setUpUsersInTable(QTableWidget& table, std::vector<UserShortInfo>& usiContextVec)
{
    this->tableClear(table);

    const short int tCols = USERS_TABLE_COLS;
    const int tRows = usiContextVec.size();

    table.setColumnCount(tCols);
    table.setRowCount(tRows);

    table.setHorizontalHeaderLabels(usersTableLabels);

    int row = 0;
    for(auto &usiItem : usiContextVec)
    {
        const QString nickname = usiItem.nickname;
        const QString rpCountString = QString::number(usiItem.rpCount);
        const QString playerIsReady = usiItem.isReady ? userIsReady : userNotReady;

        QTableWidgetItem* items[] = {
                                        new QTableWidgetItem(nickname),
                                        new QTableWidgetItem(rpCountString),
                                        new QTableWidgetItem(playerIsReady),
                                    };

        items[READY_COL]->setData(Qt::BackgroundRole, usiItem.isReady ? QColorConstants::Svg::lightgreen
                                                                      : QColorConstants::Svg::lightgrey);

        for(auto &i : items)
        {
            i->setTextAlignment(Qt::AlignCenter);
            if(usiItem.uniqueId == m_context.lobbySystem.ownerUniqueId)
            i->setBackground(QColorConstants::Svg::lightyellow);
                else
            i->setBackground(Qt::NoBrush);
        }

        for(short int col = 0; col < tCols; col++)
        {
            table.setItem(row, col, items[col]);
        }
        row++;
    }

    table.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    if(usiContextVec.size() >= 4)
        table.verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void LobbyWindow::tableClear(QTableWidget &table)
{
    table.clearContents();
    table.clear();
    table.setColumnCount(0);
    table.setRowCount(0);
}

void LobbyWindow::toggleLobbyVision()
{
    pServer()->get()->tryToggleLobbyVision(m_context.lobbySystem.uniqueId);
}

void LobbyWindow::startGame()
{
    pServer()->get()->tryStartGame(m_context.lobbySystem.uniqueId);
}

void LobbyWindow::applySettings()
{
    // After a completion
    pServer()->get()->tryLobbySettingsApply(m_context.lobbySystem.uniqueId);
}

void LobbyWindow::leaveLobby()
{
    if(makeDialog(BaseWin::LeaveLobby) == 0)
    {
        this->hide();
        emit goToMenuWindow();
    }
    // Make delete lobby request if host user leaves
    if(m_privilegeType == Owner)
        pServer()->get()->deleteLobbyRequest(m_context.lobbySystem.uniqueId);
}

void LobbyWindow::toggleReadyStatus()
{

    pServer()->get()->tryToggleReady(m_context.lobbySystem.uniqueId);
}

void LobbyWindow::quitApp()
{
    if(makeDialog(BaseWin::QuitApp) == 0)
    {
        // Make delete lobby request if host user leaves
        if(m_privilegeType == Owner)
            pServer()->get()->deleteLobbyRequest(m_context.lobbySystem.uniqueId);
        QCoreApplication::quit();
    }
}
