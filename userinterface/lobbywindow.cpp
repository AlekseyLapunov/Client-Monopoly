#include "lobbywindow.h"
#include "ui_lobbywindow.h"

LobbyWindow::LobbyWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                         unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                         QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LobbyWindow)
{
    if(!*newServerPtr || !*newMetaInfoPtr)
        throw std::runtime_error(ssLobbyWindowClassName + ssRuntimeErrors[PtrLinkFail]);

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
    context.usersInLobby.push_back({hostUser.nickname, hostUser.rpCount, false, hostUser.uniqueId});
    m_context = context;
    if(m_context.settings.ownerUniqueId == hostUser.uniqueId)
    {
        if(FileManager::isLastSettingsFileExists())
        {
            try
            {
                LobbySettings settingsFromLocal = FileManager::getLastSettingsFromLocal();
                m_lastSettings.softOverride(settingsFromLocal);
                m_context.settings.softOverride(m_lastSettings);
                overwriteSettingsInputs(m_lastSettings);
                applySettings();
            }
            catch (std::runtime_error &e)
            {
                qDebug() << e.what();
            }
        }
        else m_lastSettings = context.settings;
    }
    else
        m_lastSettings = context.settings;
}

void LobbyWindow::windowDataRefresh()
{
    ///if(!isEnabled()) return;

    pUserMetaInfo()->get()->setHostInfo(pServer()->get()->getCurrentHostInfo());
    ///m_context = pServer()->get()->getCurrentLobbyContext(m_context.lobbySystem.uniqueId);
    ui->setupUi(this);
    setDisabled(true);
    setUpSettingsInputs();
    setUpUsersInTable(*ui->tUsers, m_context.usersInLobby);
    setUpByPrivilege();
    setEnabled(true);
}

void LobbyWindow::show()
{
    windowDataRefresh();
    ui->bApplySettings->setEnabled(false);
    ui->bRestoreLastSettings->setEnabled(false);
    QWidget::show();
}

void LobbyWindow::definePrivilege()
{
    int uniqueHostId = pUserMetaInfo()->get()->getHostInfo().uniqueId;
    // If lobby is ranked
    if(m_context.settings.ownerUniqueId < 0)
        m_privilegeType = RankedGuest;
    else
    // If host user is the lobby host
    if(m_context.settings.ownerUniqueId == uniqueHostId)
    {
        m_privilegeType = Owner;
    }
    else
        m_privilegeType = Guest;
}

void LobbyWindow::setUpByPrivilege()
{
    QFont font = ui->lLobbyUniqueId->font();
    font.setPointSize(font.pointSize()*1.3);
    definePrivilege();
    switch (m_privilegeType)
    {
    case RankedGuest:
        setButtonsVisibility(false);
        ui->lLobbySettings->setText(ssRankedLobby);
        ui->lLobbyUniqueId->setText(ssAverageRp + countAverageRp());
        ui->lLobbyUniqueId->setFont(font);
        ui->lPasswordConst->setVisible(false);
        ui->lePassword->setVisible(false);
        ui->lLobbyNameConst->setVisible(false);
        ui->leLobbyName->setVisible(false);
        ui->lMaxPlayersConst->setVisible(false);
        ui->sbMaxPlayers->setVisible(false);
        ui->bToggleReady->setVisible(false);
        setWindowTitle(ssRankedLobby);
        ui->aSetRankedSettings->setVisible(false);
        ui->aImportFromFile->setVisible(false);
        ui->aExportToFile->setVisible(false);
        ui->menuSettings->setDisabled(true);
        break;
    case Guest:
        setButtonsVisibility(false);
        setWindowTitle(ssLobbyOfPlayer + "\""
                       + findOwnerNickname(m_context.settings.ownerUniqueId)
                       + "\"");
        ui->aSetRankedSettings->setVisible(false);
        ui->aImportFromFile->setVisible(false);
        ui->aExportToFile->setVisible(true);
        break;
    case Owner:
        setButtonsVisibility(true);
        setSettingsInputsAccessibility(true);
        ui->bLeaveLobby->setText(ssDeleteLobbyText);
        ui->aLeaveLobby->setText(ssDeleteLobbyText);
        setWindowTitle(ssMyLobby);
        ui->aSetRankedSettings->setVisible(true);
        ui->aImportFromFile->setVisible(true);
        ui->aExportToFile->setVisible(true);
        break;
    default:
        break;
    }
}

void LobbyWindow::setButtonsVisibility(bool areVisible)
{
    ui->bApplySettings->setVisible(areVisible);
    ui->bRestoreLastSettings->setVisible(areVisible);
    ui->bStartGame->setVisible(areVisible);

    // These should be always visible by default
    ui->bToggleReady->setVisible(true);
    ui->bLeaveLobby->setVisible(true);
}

void LobbyWindow::setSettingsInputsAccessibility(bool areAccessible)
{
    ui->leLobbyName->setReadOnly(!areAccessible);
    ui->lePassword->setReadOnly(!areAccessible);
    ui->sbMaxPlayers->setReadOnly(!areAccessible);
    ui->sbMaxTurns->setReadOnly(!areAccessible);
    ui->sbTurnTime->setReadOnly(!areAccessible);
    ui->dsbMaxBalance->setReadOnly(!areAccessible);
    ui->chbIsBalanceInfinite->setEnabled(areAccessible);
    ui->chbAreTurnsInfinite->setEnabled(areAccessible);
}

void LobbyWindow::setUpSettingsInputs()
{
    ui->lLobbyUniqueId->setText(ssLobbyIdPrefix + m_context.settings.uniqueId);
    ui->leLobbyName->setText(m_context.settings.lobbyName);
    ui->leLobbyName->setReadOnly(true);
    ui->lePassword->setText(m_context.settings.lobbyPassword);
    ui->lePassword->setReadOnly(true);
    ui->sbMaxPlayers->setValue(m_context.settings.maxPlayersCount);
    ui->sbMaxPlayers->setReadOnly(true);
    ui->sbTurnTime->setValue(m_context.settings.turnTime);
    ui->sbTurnTime->setReadOnly(true);

    ui->sbMaxTurns->setValue(m_context.settings.maxTurns);
    ui->sbMaxTurns->setReadOnly(true);

    ui->dsbMaxBalance->setValue(m_context.settings.maxMoney);
    ui->dsbMaxBalance->setReadOnly(true);

    ui->chbAreTurnsInfinite->setChecked(m_context.settings.areMaxTurnsInfinite);
    ui->sbTurnTime->setDisabled(m_context.settings.areMaxTurnsInfinite);

    ui->chbIsBalanceInfinite->setChecked(m_context.settings.isMaxMoneyInfinite);
    ui->dsbMaxBalance->setDisabled(m_context.settings.isMaxMoneyInfinite);

    ui->chbAreTurnsInfinite->setDisabled(true);
    ui->chbIsBalanceInfinite->setDisabled(true);

}

void LobbyWindow::setUpUsersInTable(QTableWidget& table, std::vector<UserShortInfo>& usiContextVec)
{
    tableClear(table);

    const short int tCols = USERS_TABLE_COLS;
    const int tRows = usiContextVec.size();

    table.setColumnCount(tCols);
    table.setRowCount(tRows);
    table.hideColumn(PLAYER_UNIQUE_ID_COL);

    table.setHorizontalHeaderLabels(ssUsersTableLabels);

    int row = 0;
    for(auto &usiItem : usiContextVec)
    {
        const QString nickname = usiItem.nickname;
        const QString rpCountString = QString::number(usiItem.rp);
        const QString playerIsReady = usiItem.isReady ? ssUserIsReady : ssUserNotReady;
        const QString playerUniqueId = QString::number(usiItem.uniqueId);

        QTableWidgetItem* items[] = {
                                        new QTableWidgetItem(nickname),
                                        new QTableWidgetItem(rpCountString),
                                        new QTableWidgetItem(playerIsReady),
                                        new QTableWidgetItem(playerUniqueId)
                                    };

        if(usiItem.isReady)
            items[READY_COL]->setBackground(QColorConstants::Svg::lightgreen);

        for(auto &i : items)
        {
            i->setTextAlignment(Qt::AlignCenter);
            if(usiItem.uniqueId == m_context.settings.ownerUniqueId)
                i->setBackground(QColorConstants::Svg::lightyellow);
        }

        for(short int col = 0; col < tCols; col++)
        {
            table.setItem(row, col, items[col]);
        }
        row++;
    }

    table.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    if(m_context.settings.ownerUniqueId < 0)
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
    for(auto &i : m_context.usersInLobby)
    {
        sum += i.rp;
    }
    averageRpOutput = QString::number(sum / m_context.usersInLobby.size());
    return averageRpOutput;
}

QString LobbyWindow::findOwnerNickname(int ownerId)
{
    QString ownerNicknameOutput = "";
    for(auto &i : m_context.usersInLobby)
    {
        if(i.uniqueId == ownerId)
        {
            ownerNicknameOutput = i.nickname;
            break;
        }
    }
    return ownerNicknameOutput;
}

void LobbyWindow::startGame()
{
    ui->bStartGame->setDisabled(true);

    if(ui->bApplySettings->isEnabled())
        if(makeDialog(BaseWin::StartGameSettingsNotApplied, "", this) != 0)
        {
            ui->bStartGame->setDisabled(false);
            return;
        }

    windowDataRefresh();

    if(m_context.usersInLobby.size() < MIN_PLAYERS_COUNT)
    {
        execErrorBox(ssNotEnoughPlayers, this);
        ui->bStartGame->setDisabled(false);
        return;
    }

    if(!checkIfEveryoneReady())
        if(makeDialog(BaseWin::StartGameNotReady, "", this) != 0)
        {
            ui->bStartGame->setDisabled(false);
            return;
        }

    try
    {
        pServer()->get()->tryStartGame(m_context.settings.uniqueId, m_lastSettings);
        m_context.settings.softOverride(m_lastSettings);
        setDisabled(true);
    }
    catch (std::exception &e)
    {
        execErrorBox(e.what(), this);
    }

    ui->bStartGame->setDisabled(false);
}

void LobbyWindow::applySettings()
{
    try
    {
        LobbySettings tempSettings = makeSettingsObjectByInputs();
        pServer()->get()->tryLobbySettingsApply(m_context.settings.uniqueId, tempSettings);
        ui->bApplySettings->setDisabled(true);
        ui->bRestoreLastSettings->setDisabled(true);
        m_lastSettings.softOverride(tempSettings);
        FileManager::saveLastSettingsToLocal(m_lastSettings);
    }
    catch (std::exception &e)
    {
        execErrorBox(e.what(), this);
    }
}

LobbySettings LobbyWindow::makeSettingsObjectByInputs()
{
    return  {
                m_context.settings.uniqueId,
                ui->leLobbyName->text(),
                ui->lePassword->text(),
                (short) ui->sbMaxPlayers->value(),
                m_context.settings.ownerUniqueId,
                m_context.settings.isTimerActive,
                m_context.settings.sessionAddress,
                m_context.settings.sessionPort,

                (short)ui->sbTurnTime->value(),
                (float)ui->dsbMaxBalance->value(),
                ui->chbIsBalanceInfinite->isChecked(),
                (short)ui->sbMaxTurns->value(),
                ui->chbAreTurnsInfinite->isChecked()
            };
}

void LobbyWindow::overwriteSettingsInputs(LobbySettings &overwriteBy)
{
    if(!overwriteBy.lobbyName.isEmpty())
        ui->leLobbyName->setText(overwriteBy.lobbyName);
    if(!overwriteBy.lobbyPassword.isEmpty())
        ui->lePassword->setText(overwriteBy.lobbyPassword);
    if(overwriteBy.maxPlayersCount >= MIN_PLAYERS_COUNT)
        ui->sbMaxPlayers->setValue(overwriteBy.maxPlayersCount);

    ui->sbTurnTime->setValue(overwriteBy.turnTime);
    ui->dsbMaxBalance->setValue(overwriteBy.maxMoney);
    ui->chbIsBalanceInfinite->setChecked(overwriteBy.isMaxMoneyInfinite);
    ui->sbMaxTurns->setValue(overwriteBy.maxTurns);
    ui->chbAreTurnsInfinite->setChecked(overwriteBy.areMaxTurnsInfinite);
}

bool LobbyWindow::checkIfEveryoneReady()
{
    for(auto &i : m_context.usersInLobby)
        if(!i.isReady && (i.uniqueId != m_context.settings.ownerUniqueId))
            return false;
    return true;
}

void LobbyWindow::leaveLobby()
{
    if(makeDialog(BaseWin::LeaveLobby, "", this) == 0)
    {
        hide();
        emit goToMenuWindow();
    }
    // Make delete lobby request if host user leaves
    if(m_privilegeType == Owner)
        pServer()->get()->deleteLobbyRequest(m_context.settings.uniqueId);
}

void LobbyWindow::toggleReadyStatus()
{
    ui->bToggleReady->setDisabled(true);
    int hostUniqueId = pUserMetaInfo()->get()->getHostInfo().uniqueId;
    for(short i = 0; i < (short) m_context.usersInLobby.size(); i++)
    {
        if(m_context.usersInLobby.at(i).uniqueId == hostUniqueId)
        {
            m_context.usersInLobby.at(i).isReady = !m_context.usersInLobby.at(i).isReady;
            break;
        }
    }
    try
    {
        pServer()->get()->tryToggleReady(m_context.settings.uniqueId);
        setUpUsersInTable(*ui->tUsers, m_context.usersInLobby);
    }
    catch (std::exception &e)
    {
        execErrorBox(e.what(), this);
    }
    ui->bToggleReady->setDisabled(false);
}

void LobbyWindow::quitApp()
{
    QCoreApplication::quit();
}

void LobbyWindow::settingsChangesDetected()
{
    LobbySettings currentSettings = makeSettingsObjectByInputs();
    bool flag = (m_lastSettings != currentSettings);
    ui->bApplySettings->setEnabled(flag);
    ui->bRestoreLastSettings->setEnabled(flag);
}

void LobbyWindow::toggleMaxBalanceAccessibility()
{
    ui->dsbMaxBalance->setDisabled(ui->chbIsBalanceInfinite->isChecked());
}

void LobbyWindow::toggleMaxTurnsAccessibility()
{
    ui->sbMaxTurns->setDisabled(ui->chbAreTurnsInfinite->isChecked());
}

void LobbyWindow::restoreLastSettings()
{
    if(makeDialog(BaseWin::RestoreSettings, "", this) == 0)
    {
        overwriteSettingsInputs(m_lastSettings);
        applySettings();
        ui->bRestoreLastSettings->setDisabled(true);
    }
}

void LobbyWindow::applyRankedSettings()
{
    try
    {
        LobbySettings trueRankedSettings = FileManager::getRankedSettingsFromLocal();
        overwriteSettingsInputs(trueRankedSettings);
    }
    catch (std::exception &e)
    {
        execErrorBox(e.what(), this);
        return;
    }
    ui->bApplySettings->setEnabled(true);
    ui->bRestoreLastSettings->setEnabled(true);
}

void LobbyWindow::exportSettingsToFile()
{
    if(ui->bApplySettings->isEnabled())
        if(makeDialog(BaseWin::ExportSettingsNotApplied, "", this) != 0)
            return;

    try
    {
        FileManager::manageSettingsExport(m_lastSettings, this);
    }
    catch (std::exception &e)
    {
        execErrorBox(e.what(), this);
        return;
    }
}

void LobbyWindow::importSettingsFromFile()
{
    bool gotSettings = false;
    LobbySettings importedSettings;

    try
    {
        importedSettings = FileManager::manageSettingsImport(gotSettings, this);
    }
    catch (std::exception &e)
    {
        execErrorBox(e.what(), this);
        return;
    }

    if(!gotSettings)
        return;

    overwriteSettingsInputs(importedSettings);
    ui->bApplySettings->setEnabled(true);
    ui->bRestoreLastSettings->setEnabled(true);
}

void LobbyWindow::reactToUserSelect(QTableWidgetItem *item)
{
    int hostUniqueId = pUserMetaInfo()->get()->getHostInfo().uniqueId;
    if(m_context.settings.ownerUniqueId != hostUniqueId)
        return;

    int selectedUniqueId = ui->tUsers->item(item->row(), PLAYER_UNIQUE_ID_COL)->text().toInt();
    if(selectedUniqueId == hostUniqueId)
        return;

    QString selectedNickname = ui->tUsers->item(item->row(), NICKNAME_COL)->text();
    short dialogAnswer = makeDialog(BaseWin::PlayerSelected, selectedNickname, this);
    enum DialogAnswerCodes { Kick, Promote, Cancel };

    try
    {
        switch (dialogAnswer)
        {
        case DialogAnswerCodes::Kick:
            pServer()->get()->tryKickPlayer(m_context.settings.uniqueId, selectedUniqueId);
            break;
        case DialogAnswerCodes::Promote:
            if(makeDialog(BaseWin::PlayerPromoteConfirmation, selectedNickname, this) != 0)
                return;
            pServer()->get()->tryPromotePlayer(m_context.settings.uniqueId, selectedUniqueId);
            break;
        case DialogAnswerCodes::Cancel:
            return;
        default:
            return;
        }
    }
    catch (std::exception &e)
    {
        execErrorBox(e.what(), this);
        return;
    }
}

void LobbyWindow::closeEvent(QCloseEvent *event)
{
    quitAppDialog();
    event->ignore();
}

void LobbyWindow::quitAppDialog()
{
    if(makeDialog(BaseWin::QuitApp, "", this) == 0)
    {
        // Make delete lobby request if host user leaves
        if(m_privilegeType == Owner)
            pServer()->get()->deleteLobbyRequest(m_context.settings.uniqueId);
        QCoreApplication::quit();
    }
}
