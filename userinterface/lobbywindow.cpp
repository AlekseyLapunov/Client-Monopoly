#include "lobbywindow.h"
#include "ui_lobbywindow.h"

LobbyWindow::LobbyWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                         unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                         unique_ptr<GameManagerWindow> &newGameManagerPtr,
                         QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LobbyWindow)
{
    if(!*newServerPtr || !*newMetaInfoPtr || !newGameManagerPtr)
        throw std::runtime_error(ssClassNames[LobbyWindowCN] + ssErrorsContent[PtrLinkFail]);

    setupPointers(*newServerPtr, *newMetaInfoPtr);
    this->m_pGameManager = &newGameManagerPtr;

    connect(&refreshDataTimer, &QTimer::timeout, this, &LobbyWindow::windowDataRefresh);

    ui->setupUi(this);
}

LobbyWindow::~LobbyWindow()
{
    delete ui;
}

void LobbyWindow::setFirstContext(const LobbyFullInfo context)
{
    HostUserData hostUser = pUserMetaInfo()->get()->getHostInfo();
    m_context = context;
    m_lastSettings = m_context.settings;
}

void LobbyWindow::windowDataRefresh()
{
    if(!isEnabled() || isHidden())
        return;

    m_context.usersInLobby.clear();
    m_context.usersInLobby.resize(0);

    ResponseFromServerComm<LobbyFullInfo> response;
    response = pServer()->get()->getInfoLobby();

    switch (response.responseFlag)
    {
    case AllGoodRf:
        timedOutCounter = 0;
        m_context = response.payload;
        setUpUsersInTable(*ui->tUsers, m_context.usersInLobby);
        setUpByPrivilege();
        overwriteSettingsInputs(m_context.settings);
        checkIfNeedToStartGame();
        return;
    case UnauthorizeRf:
        logoutBackToLoginWindow();
        return;
    case TimedOutRf:
        timedOutCounter++;
        checkTimedOutCounter();
        return;
    default:
        execErrorBox(QString("%1%2").arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                                         QString::fromStdString(ssErrorsContent[GetLobbyInfoFail])),
                     this);
        switchBackToMenuWindow();
        return;
    }
}

void LobbyWindow::show(const LobbyFullInfo firstContext)
{
    refreshDataTimer.stop();

    ui->lGameBeginsIn->setVisible(false);
    ui->lSecondsToStart->setVisible(false);
    setFirstContext(firstContext);
    setUpSettingsInputs();
    setUpRegExps();
    ui->bApplySettings->setEnabled(false);
    ui->bRestoreLastSettings->setEnabled(false);
    setUpByPrivilege(); // !
    setUpUsersInTable(*ui->tUsers, m_context.usersInLobby);
    setUpByPrivilege();
    overwriteSettingsInputs(m_context.settings);
    checkIfNeedToStartGame();

    if(!FileManager::checkUserMetaIntegrity())
    {
        logoutBackToLoginWindow();
        return;
    }

    uint8_t host3dDicePreference = FileManager::getUser3dDicePreference();
    if(host3dDicePreference == FileManager::DicePrefNotStated)
        ui->aUse3dDice->setChecked(true);
    else
        ui->aUse3dDice->setChecked(host3dDicePreference);

    ResponseFromServerComm<HostUserData> response;
    response = pServer()->get()->getCurrentHostInfo(false);

    switch (response.responseFlag)
    {
    case AllGoodRf:
        timedOutCounter = 0;
        pUserMetaInfo()->get()->setHostInfo(response.payload);
        setEnabled(true);
        windowDataRefresh();
        refreshDataTimer.start(REFRESH_LOBBY_INSIDE_DATA_EVERY_N_MS);
        QMainWindow::show();
        return;
    case UnauthorizeRf:
        logoutBackToLoginWindow();
        return;
    case TimedOutRf:
        timedOutCounter++;
        checkTimedOutCounter();
        return;
    default:
        execErrorBox(QString("%1%2").arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                                         QString::fromStdString(ssErrorsContent[GetHostInfoFail])),
                     this);
        switchBackToMenuWindow();
        return;
    }
}

void LobbyWindow::hide()
{
    setDisabled(true);
    refreshDataTimer.stop();
    QMainWindow::hide();
}

short LobbyWindow::definePrivilege()
{
    int uniqueHostId = pUserMetaInfo()->get()->getHostInfo().uniqueId;

    if(m_context.settings.type == RANKED)
        return RankedJoinedUser;
    else if(m_context.settings.ownerUniqueId == uniqueHostId)
        return Owner;
    else
        return JoinedUser;
}

void LobbyWindow::setUpByPrivilege()
{
    QFont font = ui->lLobbyUniqueId->font();
    font.setPointSize(font.pointSize()*1.3);
    switch (definePrivilege())
    {
    case RankedJoinedUser:
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
        ui->aSetRankedSettings->setVisible(false);
        ui->aImportFromFile->setVisible(false);
        ui->aExportToFile->setVisible(false);
        ui->aChangeNickname->setVisible(true);
        ui->aUse3dDice->setVisible(true);
        ui->aDeleteLobby->setDisabled(true);

        setButtonsVisibility(false);
        setWindowTitle(ssRankedLobby);
        setSettingsInputsAccessibility(false);
        break;
    case JoinedUser:
        ui->aSetRankedSettings->setVisible(false);
        ui->aImportFromFile->setVisible(false);
        ui->aExportToFile->setVisible(true);
        ui->aDeleteLobby->setDisabled(true);
        ui->aChangeNickname->setVisible(true);
        ui->aUse3dDice->setVisible(true);

        setButtonsVisibility(false);
        setWindowTitle(QString("%1\"%2\"")
                       .arg(ssLobbyOfPlayer,
                            findOwnerNickname(m_context.settings.ownerUniqueId)));
        setSettingsInputsAccessibility(false);
        break;
    case Owner:
        ui->aSetRankedSettings->setVisible(true);
        ui->aImportFromFile->setVisible(true);
        ui->aExportToFile->setVisible(true);
        ui->aDeleteLobby->setEnabled(true);
        ui->aChangeNickname->setVisible(true);
        ui->aUse3dDice->setVisible(true);

        setButtonsVisibility(true);
        setSettingsInputsAccessibility(true);
        manageCheckBoxes();

        setWindowTitle(ssMyLobby);
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
    ui->bDeleteLobby->setVisible(areVisible);
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
    ui->sbMaxBalance->setReadOnly(!areAccessible);
    ui->chbIsBalanceInfinite->setEnabled(areAccessible);
    ui->chbAreTurnsInfinite->setEnabled(areAccessible);
}

void LobbyWindow::setUpSettingsInputs()
{
    ui->lLobbyUniqueId->setText(ssLobbyIdPrefix + QString::number(m_context.settings.uniqueId));

    overwriteSettingsInputs(m_context.settings);

    ui->leLobbyName->setMaxLength(LOBBY_NAME_MAX_LEN);
    ui->leLobbyName->setReadOnly(true);

    ui->lePassword->setMaxLength(LOBBY_PASSWORD_MAX_LEN);
    ui->lePassword->setReadOnly(true);

    ui->sbMaxPlayers->setReadOnly(true);

    ui->chbAreTurnsInfinite->setDisabled(true);

    ui->chbIsBalanceInfinite->setDisabled(true);

    ui->sbTurnTime->setReadOnly(true);

    ui->sbMaxTurns->setReadOnly(true);
    ui->sbMaxTurns->setDisabled(m_context.settings.areMaxTurnsInfinite);

    ui->sbMaxBalance->setReadOnly(true);
    ui->sbMaxBalance->setDisabled(m_context.settings.isMaxMoneyInfinite);
}

void LobbyWindow::setUpRegExps()
{
    QRegularExpression* pRegExp[] = {
                                        new QRegularExpression(ssRegExps[LobbyNameRegExp]),
                                        new QRegularExpression(ssRegExps[LobbyPasswordRegExp])
                                    };

    QRegularExpressionValidator* pLobbyFilterValidator[] = {
                                                                new QRegularExpressionValidator(*pRegExp[0], this),
                                                                new QRegularExpressionValidator(*pRegExp[1], this)
                                                           };

    ui->leLobbyName->setValidator(pLobbyFilterValidator[0]);
    ui->lePassword->setValidator(pLobbyFilterValidator[1]);
}

void LobbyWindow::setUpUsersInTable(QTableWidget& table, std::vector<UserShortInfo>& usiContextVec)
{
    tableClear(table);

    const uint8_t tCols = USERS_TABLE_COLS;
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

        for(uint8_t col = 0; col < tCols; col++)
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

    table.sortByColumn(NICKNAME_COL, Qt::AscendingOrder);
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
    int sum = 0;
    for(auto &i : m_context.usersInLobby)
    {
        sum += i.rp;
    }
    return QString::number(sum / m_context.usersInLobby.size());
}

QString LobbyWindow::findOwnerNickname(int ownerId)
{
    QString ownerNicknameOutput = "";
    for(auto &user : m_context.usersInLobby)
    {
        if(user.uniqueId == ownerId)
        {
            ownerNicknameOutput = user.nickname;
            break;
        }
    }
    return ownerNicknameOutput;
}

void LobbyWindow::startGame()
{
#ifdef DEBUG_FAST_GAME_START
    hide();
    m_pGameManager->get()->show();
    return;
#endif
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

    ResponseFromServerComm<void*> response;
    response = pServer()->get()->runGame();

    switch (response.responseFlag)
    {
    case AllGoodRf:
        timedOutCounter = 0;
        m_context.settings.softOverride(m_lastSettings);
        ui->bStartGame->setDisabled(false);
        return;
    case UnauthorizeRf:
        logoutBackToLoginWindow();
        return;
    case TimedOutRf:
        timedOutCounter++;
        checkTimedOutCounter();
        ui->bStartGame->setDisabled(false);
        return;
    default:
        ui->bStartGame->setDisabled(false);
        execErrorBox(QString("%1%2")
                     .arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                          QString::fromStdString(ssErrorsContent[StartGameFail])),
                     this);
        switchBackToMenuWindow();
        return;
    }
}

void LobbyWindow::applySettings()
{
    checkMaxPlayers();
    checkLimitationChecks();
    ui->bApplySettings->setDisabled(true);
    ui->bRestoreLastSettings->setDisabled(true);
    LobbySettings tempSettings = makeSettingsObjectByInputs();
    FileManager::saveLastSettingsToLocal(tempSettings);

    ResponseFromServerComm<void*> response;
    response = pServer()->get()->updateLobbySettings(tempSettings);

    switch (response.responseFlag)
    {
    case AllGoodRf:
        timedOutCounter = 0;

        ui->bApplySettings->setDisabled(true);
        ui->bRestoreLastSettings->setDisabled(true);
        m_lastSettings.softOverride(tempSettings);
        return;
    case UnauthorizeRf:
        ui->bApplySettings->setDisabled(false);
        ui->bRestoreLastSettings->setDisabled(false);
        logoutBackToLoginWindow();
        return;
    case TimedOutRf:
        ui->bApplySettings->setDisabled(false);
        ui->bRestoreLastSettings->setDisabled(false);
        timedOutCounter++;
        checkTimedOutCounter();
        return;
    default:
        ui->bApplySettings->setDisabled(false);
        ui->bRestoreLastSettings->setDisabled(false);
        execErrorBox(QString("%1%2")
                     .arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                          QString::fromStdString(ssErrorsContent[ApplySettingsFail])),
                     this);
        switchBackToMenuWindow();
        return;
    }
}

LobbySettings LobbyWindow::makeSettingsObjectByInputs()
{
    return  {
                m_context.settings.uniqueId,
                ui->leLobbyName->text(),
                ui->lePassword->text(),
                (int8_t) ui->sbMaxPlayers->value(),
                m_context.settings.ownerUniqueId,
                m_context.settings.isTimerActive,
                m_context.settings.sessionAddress,
                m_context.settings.sessionPort,

                (int8_t) ui->sbTurnTime->value(),
                ui->sbMaxBalance->value(),
                ui->chbIsBalanceInfinite->isChecked(),
                (uint8_t)ui->sbMaxTurns->value(),
                ui->chbAreTurnsInfinite->isChecked(),
                m_context.settings.type
            };
}

void LobbyWindow::overwriteSettingsInputs(LobbySettings &overwriteBy)
{
    if((pUserMetaInfo()->get()->getHostInfo().uniqueId == m_context.settings.ownerUniqueId)
            && (ui->bApplySettings->isEnabled() || ui->bRestoreLastSettings->isEnabled()))
        return;

    if(!overwriteBy.lobbyName.isEmpty())
        ui->leLobbyName->setText(overwriteBy.lobbyName);
    if(!overwriteBy.lobbyPassword.isEmpty())
        ui->lePassword->setText(overwriteBy.lobbyPassword);
    if(overwriteBy.maxPlayersCount >= MIN_PLAYERS_COUNT)
        ui->sbMaxPlayers->setValue(overwriteBy.maxPlayersCount);

    ui->sbTurnTime->setValue(overwriteBy.turnTime);
    ui->sbMaxBalance->setValue(overwriteBy.maxMoney);
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

void LobbyWindow::checkMaxPlayers()
{
    int playersCount = m_context.usersInLobby.size();
    if(ui->sbMaxPlayers->value() < playersCount)
        ui->sbMaxPlayers->setValue(playersCount);
}

void LobbyWindow::switchBackToMenuWindow()
{
    hide();

    pServer()->get()->disconnectFromLobby();
    emit goToMenuWindow();
}

void LobbyWindow::checkLimitationChecks()
{
    if(ui->chbAreTurnsInfinite->isChecked() &&
       ui->chbIsBalanceInfinite->isChecked())
    {
        ui->chbIsBalanceInfinite->setChecked(true);
        ui->chbAreTurnsInfinite->setChecked(false);
    }
}

void LobbyWindow::logoutBackToLoginWindow()
{
    hide();
    setDisabled(true);

    emit goToMenuWindow();
    emit initLogoutChain();
}

void LobbyWindow::checkTimedOutCounter()
{
    if(timedOutCounter >= MAX_TIMED_OUT_COUNTER)
    {
        timedOutCounter = 0;
        logoutBackToLoginWindow();
    }
}

void LobbyWindow::manageCheckBoxes()
{
    if(m_context.settings.ownerUniqueId == pUserMetaInfo()->get()->getHostInfo().uniqueId)
    {
        if(ui->chbIsBalanceInfinite->isChecked())
        {
            ui->chbAreTurnsInfinite->setChecked(false);
            ui->chbAreTurnsInfinite->setDisabled(true);
        }
        else
            ui->chbAreTurnsInfinite->setEnabled(true);

        if(ui->chbAreTurnsInfinite->isChecked())
        {
            ui->chbIsBalanceInfinite->setChecked(false);
            ui->chbIsBalanceInfinite->setDisabled(true);
        }
        else
            ui->chbIsBalanceInfinite->setEnabled(true);
    }
}

void LobbyWindow::checkIfNeedToStartGame()
{
    if(!m_context.settings.isTimerActive)
    {
        subTimerToStartGame.stop();
        subTimerToStartGame.disconnect();
        timerToStartGame.stop();
        timerToStartGame.disconnect();
        ui->bToggleReady->setEnabled(true);
        ui->lGameBeginsIn->setVisible(false);
        ui->lSecondsToStart->setVisible(false);
        timerAlreadyActive = false;
        return;
    }
    if(timerAlreadyActive)
        return;

    timerAlreadyActive = true;
    ui->bToggleReady->setDisabled(true);

    ui->lGameBeginsIn->setVisible(true);
    ui->lSecondsToStart->setText(QString::number(TIME_TO_START_GAME_MS/1000));

    timerToStartGame.setSingleShot(true);
    timerToStartGame.start(TIME_TO_START_GAME_MS);

    subTimerToStartGame.setSingleShot(false);
    subTimerToStartGame.start(1000);

    connect(&subTimerToStartGame, &QTimer::timeout, [=]()
    {
        ui->lSecondsToStart->setText(QString::number(timerToStartGame.remainingTime()/1000));
    });

    connect(&timerToStartGame, &QTimer::timeout, [=]()
    {
        subTimerToStartGame.stop();
        openGameWindow();
    });
}

void LobbyWindow::leaveLobby()
{
    if(makeDialog(BaseWin::LeaveLobby, "", this) == 0)
    {
        switchBackToMenuWindow();
    }
}

void LobbyWindow::toggleReadyStatus()
{
    ui->bToggleReady->setDisabled(true);

    pServer()->get()->switchReadiness();

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

    manageCheckBoxes();
}

void LobbyWindow::toggleMaxBalanceAccessibility()
{
    ui->sbMaxBalance->setDisabled(ui->chbIsBalanceInfinite->isChecked());
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
        checkMaxPlayers();
        applySettings();
        ui->bRestoreLastSettings->setDisabled(true);
    }
}

void LobbyWindow::applyRankedSettings()
{
    LobbySettings trueRankedSettings = FileManager::getRankedSettingsFromLocal();
    overwriteSettingsInputs(trueRankedSettings);

    ui->bApplySettings->setEnabled(true);
    ui->bRestoreLastSettings->setEnabled(true);
}

void LobbyWindow::exportSettingsToFile()
{
    if(ui->bApplySettings->isEnabled())
    {
        if(makeDialog(BaseWin::ExportSettingsNotApplied, "", this) == 0)
        {
            FileManager::manageSettingsExport(m_lastSettings, this);
            return;
        }
    }
    else
    {
        FileManager::manageSettingsExport(m_context.settings, this);
        return;
    }
}

void LobbyWindow::importSettingsFromFile()
{
    bool gotSettings = false;

    LobbySettings importedSettings = FileManager::manageSettingsImport(gotSettings, this);

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
    short dialogAnswer = makeDialog(BaseWin::ActionOnPlayerSelected, selectedNickname, this);
    enum DialogAnswerCodes { Kick, Promote, Cancel };

    switch (dialogAnswer)
    {
        case DialogAnswerCodes::Kick:
        {
            if(makeDialog(BaseWin::PlayerKickConfirmation, selectedNickname, this) != 0)
                return;

            ResponseFromServerComm<void*> response;
            response = pServer()->get()->kickPlayer(selectedUniqueId);

            switch (response.responseFlag)
            {
            case AllGoodRf:
                timedOutCounter = 0;
                windowDataRefresh();
                return;
            case UnauthorizeRf:
                timedOutCounter = 0;
                logoutBackToLoginWindow();
                return;
            case TimedOutRf:
                timedOutCounter++;
                checkTimedOutCounter();
                return;
            default:
                execErrorBox(QString("%1%2")
                             .arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                                  QString::fromStdString(ssErrorsContent[KickPlayerFail])));
                switchBackToMenuWindow();
                return;
            }
            return;
        }
        case DialogAnswerCodes::Promote:
        {
            if(makeDialog(BaseWin::PlayerPromoteConfirmation, selectedNickname, this) != 0)
                return;

            ResponseFromServerComm<void*> response;
            response = pServer()->get()->raisePlayer(selectedUniqueId);

            switch (response.responseFlag)
            {
            case AllGoodRf:
                timedOutCounter = 0;
                m_context.settings.ownerUniqueId = selectedUniqueId;
                windowDataRefresh();
                return;
            case UnauthorizeRf:
                timedOutCounter = 0;
                logoutBackToLoginWindow();
                return;
            case TimedOutRf:
                timedOutCounter++;
                checkTimedOutCounter();
                return;
            default:
                execErrorBox(QString("%1%2")
                             .arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                                  QString::fromStdString(ssErrorsContent[PromotePlayerFail])));
                switchBackToMenuWindow();
                return;
            }
            return;
        }
        case DialogAnswerCodes::Cancel:
            return;
        default:
            return;
    }
}

void LobbyWindow::togglePasswordLineEditEcho()
{
    if(ui->lePassword->echoMode() == QLineEdit::Password)
        ui->lePassword->setEchoMode(QLineEdit::Normal);
    else ui->lePassword->setEchoMode(QLineEdit::Password);
}

void LobbyWindow::deleteLobby()
{
    if(makeDialog(BaseWin::DeleteLobby, "", this) != 0)
        return;

    if(pUserMetaInfo()->get()->getHostInfo().uniqueId == m_context.settings.ownerUniqueId)
    {
        pServer()->get()->deleteLobby();
        hide();
        emit goToMenuWindow();
        return;
    }
}

void LobbyWindow::changeNickname()
{
    MenuSubDialog *subDialog = new MenuSubDialog(this);
    subDialog->selfConfig(MenuSubDialog::ChangeNickname);
    if(subDialog->exec() == QDialog::Accepted)
        pServer()->get()->changeNickname(subDialog->nicknameValue());
    subDialog->deleteLater();
}

void LobbyWindow::apply3dDiceState()
{
    FileManager::apply3dDiceStateToLocal(ui->aUse3dDice->isChecked());
}

void LobbyWindow::openGameWindow()
{
    qDebug().noquote() << "Open game window";
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
        pServer()->get()->disconnectFromLobby();

        QCoreApplication::quit();
    }
}
