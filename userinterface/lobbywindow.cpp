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
    HostUserData hostUser = this->pUserMetaInfo()->get()->getHostInfo();
    context.usersInTable.push_back({hostUser.userName, hostUser.userRpCount, false, hostUser.uniqueUserId});
    m_context = context;
    m_lastSettings = {context.lobbySystem, context.gameSettings};
}

void LobbyWindow::windowDataRefresh()
{
    ///if(!this->isEnabled()) return;

    this->pUserMetaInfo()->get()->setHostInfo(pServer()->get()->getCurrentHostInfo());
    ///m_context = this->pServer()->get()->getCurrentLobbyContext(m_context.lobbySystem.uniqueId);
    this->ui->setupUi(this);
    this->setDisabled(true);
    this->setUpLobbySystem(m_context.lobbySystem);
    this->setUpGameSettings(m_context.gameSettings);
    this->setUpUsersInTable(*this->ui->tUsers, m_context.usersInTable);
    this->setUpByPrivilege();
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
    QFont font = this->ui->lLobbyUniqueId->font();
    font.setPointSize(font.pointSize()*1.3);
    definePrivilege();
    switch (m_privilegeType)
    {
    case RankedGuest:
        setButtonsVisibility(false);
        this->ui->lLobbySettings->setText(rankedLobby);
        this->ui->lLobbyUniqueId->setText(averageRp + countAverageRp());
        this->ui->lLobbyUniqueId->setFont(font);
        this->ui->lPasswordConst->setVisible(false);
        this->ui->lePassword->setVisible(false);
        this->ui->lLobbyNameConst->setVisible(false);
        this->ui->leLobbyName->setVisible(false);
        this->ui->lMaxPlayersConst->setVisible(false);
        this->ui->sbMaxPlayers->setVisible(false);
        this->ui->bToggleReady->setVisible(false);
        this->setWindowTitle(rankedLobby);
        break;
    case Guest:
        this->setButtonsVisibility(false);
        this->setWindowTitle(lobbyOfPlayer + "\""
                             + findOwnerNickname(m_context.lobbySystem.ownerUniqueId)
                             + "\"" + " (" + (m_context.lobbySystem.isPrivate ? lobbyHidden
                                                                             : lobbyVisible) + ")");
        break;
    case Owner:
        this->setButtonsVisibility(true);
        this->setSettingsInputsAccessibility(true);
        this->ui->bLeaveLobby->setText(deleteLobbyText);
        this->ui->aLeaveLobby->setText(deleteLobbyText);
        this->setWindowTitle(myLobby + " (" + (m_context.lobbySystem.isPrivate ? lobbyHidden
                                                                               : lobbyVisible) + ")");
        break;
    default:
        break;
    }
}

void LobbyWindow::setButtonsVisibility(bool areVisible)
{
    this->ui->bApplySettings->setVisible(areVisible);
    this->ui->bRestoreLastSettings->setVisible(areVisible);
    this->ui->bStartGame->setVisible(areVisible);
    this->ui->bToggleLobbyVision->setVisible(areVisible);
    this->ui->bToggleLobbyVision->setText(lobbyVisibility + (m_context.lobbySystem.isPrivate ? lobbyHidden
                                                                                             : lobbyVisible));

    // These should be always visible by default
    this->ui->bToggleReady->setVisible(true);
    this->ui->bLeaveLobby->setVisible(true);
}

void LobbyWindow::setSettingsInputsAccessibility(bool areAccessible)
{
    this->ui->leLobbyName->setReadOnly(!areAccessible);
    this->ui->lePassword->setReadOnly(!areAccessible);
    this->ui->sbMaxPlayers->setReadOnly(!areAccessible);
    this->ui->sbMaxTurns->setReadOnly(!areAccessible);
    this->ui->sbTurnTime->setReadOnly(!areAccessible);
    this->ui->dsbMaxBalance->setReadOnly(!areAccessible);
    this->ui->chbIsBalanceInfinite->setEnabled(areAccessible);
    this->ui->chbAreTurnsInfinite->setEnabled(areAccessible);
}

void LobbyWindow::setUpLobbySystem(LobbySystemInfo& lsiContext)
{
    this->ui->lLobbyUniqueId->setText("ID " + QString::number(lsiContext.uniqueId));
    this->ui->leLobbyName->setText(lsiContext.lobbyName);
    this->ui->leLobbyName->setReadOnly(true);
    this->ui->lePassword->setText(lsiContext.lobbyPassword);
    this->ui->lePassword->setReadOnly(true);
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

    this->ui->chbAreTurnsInfinite->setChecked(gsContext.areMaxTurnsInfinite);
    this->ui->sbTurnTime->setDisabled(gsContext.areMaxTurnsInfinite);

    this->ui->chbIsBalanceInfinite->setChecked(gsContext.isMaxMoneyInfinite);
    this->ui->dsbMaxBalance->setDisabled(gsContext.isMaxMoneyInfinite);

    this->ui->chbAreTurnsInfinite->setDisabled(true);
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

        if(usiItem.isReady)
            items[READY_COL]->setBackground(QColorConstants::Svg::lightgreen);

        for(auto &i : items)
        {
            i->setTextAlignment(Qt::AlignCenter);
            if(usiItem.uniqueId == m_context.lobbySystem.ownerUniqueId)
                i->setBackground(QColorConstants::Svg::lightyellow);
        }

        for(short int col = 0; col < tCols; col++)
        {
            table.setItem(row, col, items[col]);
        }
        row++;
    }

    table.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    if(m_context.lobbySystem.uniqueId < 0)
        table.hideColumn(READY_COL);

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

QString LobbyWindow::countAverageRp()
{
    QString averageRpOutput = "";
    int sum = 0;
    for(auto &i : m_context.usersInTable)
    {
        sum += i.rpCount;
    }
    averageRpOutput = QString::number(sum / m_context.usersInTable.size());
    return averageRpOutput;
}

QString LobbyWindow::findOwnerNickname(int ownerId)
{
    QString ownerNicknameOutput = "";
    for(auto &i : m_context.usersInTable)
    {
        if(i.uniqueId == ownerId)
        {
            ownerNicknameOutput = i.nickname;
            break;
        }
    }
    return ownerNicknameOutput;
}

void LobbyWindow::toggleLobbyVision()
{
    this->ui->bToggleLobbyVision->setDisabled(true);

    try
    {
        pServer()->get()->tryToggleLobbyVision(m_context.lobbySystem.uniqueId);
        m_context.lobbySystem.isPrivate = !m_context.lobbySystem.isPrivate;
        this->setUpByPrivilege();
    }
    catch (std::exception &e)
    {
        this->execErrorBox(e.what());
    }

    this->ui->bToggleLobbyVision->setDisabled(false);
}

void LobbyWindow::startGame()
{
    this->ui->bStartGame->setDisabled(true);

    if(ui->bApplySettings->isEnabled())
        if(makeDialog(BaseWin::StartGameSettingsNotApplied) != 0)
        {
            this->ui->bStartGame->setDisabled(false);
            return;
        }

    this->windowDataRefresh();
    this->applySettings();

    if(!checkIfEveryoneReady())
        if(makeDialog(BaseWin::StartGameNotReady) != 0)
        {
            this->ui->bStartGame->setDisabled(false);
            return;
        }

    try
    {
        pServer()->get()->tryStartGame(m_context.lobbySystem.uniqueId);
    }
    catch (std::exception &e)
    {
        this->execErrorBox(e.what());
    }

    this->ui->bStartGame->setDisabled(false);
}

void LobbyWindow::applySettings()
{
    try
    {
        LobbySettingsCombined tempSettings = makeSettingsObject();
        pServer()->get()->tryLobbySettingsApply(m_context.lobbySystem.uniqueId, tempSettings);
        this->ui->bApplySettings->setDisabled(true);
        this->ui->bRestoreLastSettings->setDisabled(true);
        m_lastSettings = tempSettings;
    }
    catch (std::exception &e)
    {
        this->execErrorBox(e.what());
    }
}

LobbySettingsCombined LobbyWindow::makeSettingsObject()
{
    return  {
                {m_context.lobbySystem.uniqueId,
                 this->ui->leLobbyName->text(),
                 this->ui->lePassword->text(),
                 (short) this->ui->sbMaxPlayers->value(),
                 m_context.lobbySystem.ownerUniqueId,
                 m_context.lobbySystem.isPrivate},

                {(short)this->ui->sbTurnTime->value(),
                 (float)this->ui->dsbMaxBalance->value(),
                 this->ui->chbIsBalanceInfinite->isChecked(),
                 (short)this->ui->sbMaxTurns->value(),
                 this->ui->chbAreTurnsInfinite->isChecked()}
            };
}

void LobbyWindow::overwriteSettingsInputs(LobbySettingsCombined &overwriteBy)
{
    this->ui->leLobbyName->setText(overwriteBy.lobbySystem.lobbyName);
    this->ui->lePassword->setText(overwriteBy.lobbySystem.lobbyPassword);
    this->ui->sbMaxPlayers->setValue(overwriteBy.lobbySystem.maxPlayersCount);

    this->ui->sbTurnTime->setValue(overwriteBy.gameSettings.turnTime);
    this->ui->dsbMaxBalance->setValue(overwriteBy.gameSettings.moneyRightBorder);
    this->ui->chbIsBalanceInfinite->setChecked(overwriteBy.gameSettings.isMaxMoneyInfinite);
    this->ui->sbMaxTurns->setValue(overwriteBy.gameSettings.turnsRightBorder);
    this->ui->chbAreTurnsInfinite->setChecked(overwriteBy.gameSettings.areMaxTurnsInfinite);
}

bool LobbyWindow::checkIfEveryoneReady()
{
    for(auto &i : m_context.usersInTable)
        if(!i.isReady && (i.uniqueId != m_context.lobbySystem.ownerUniqueId))
            return false;
    return true;
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
    this->ui->bToggleReady->setDisabled(true);
    int hostUniqueId = pUserMetaInfo()->get()->getHostInfo().uniqueUserId;
    for(short i = 0; i < (short) m_context.usersInTable.size(); i++)
    {
        if(m_context.usersInTable.at(i).uniqueId == hostUniqueId)
        {
            m_context.usersInTable.at(i).isReady = !m_context.usersInTable.at(i).isReady;
            break;
        }
    }
    try
    {
        pServer()->get()->tryToggleReady(m_context.lobbySystem.uniqueId);
        this->setUpUsersInTable(*this->ui->tUsers, m_context.usersInTable);
    }
    catch (std::exception &e)
    {
        this->execErrorBox(e.what());
        return;
    }
    this->ui->bToggleReady->setDisabled(false);
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

void LobbyWindow::settingsChangesDetected()
{
    this->ui->bApplySettings->setEnabled(true);
    this->ui->bRestoreLastSettings->setEnabled(true);
}

void LobbyWindow::toggleMaxBalanceAccessibility()
{
    this->ui->dsbMaxBalance->setDisabled(this->ui->chbIsBalanceInfinite->isChecked());
}

void LobbyWindow::toggleMaxTurnsAccessibility()
{
    this->ui->sbMaxTurns->setDisabled(this->ui->chbAreTurnsInfinite->isChecked());
}

void LobbyWindow::restoreLastSettings()
{
    if(makeDialog(BaseWin::RestoreSettings) == 0)
    {
        this->overwriteSettingsInputs(m_lastSettings);
        this->applySettings();
        this->ui->bRestoreLastSettings->setDisabled(true);
    }
}

void LobbyWindow::showAndRefresh()
{
    this->windowDataRefresh();
    this->show();
    this->ui->bApplySettings->setEnabled(false);
    this->ui->bRestoreLastSettings->setEnabled(false);
}
