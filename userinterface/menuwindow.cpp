#include "menuwindow.h"
#include "ui_menuwindow.h"

MenuWindow::MenuWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                       unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuWindow)
{
    if(!*newServerPtr || !*newMetaInfoPtr)
        throw std::runtime_error(ssClassNames[MenuWindowCN] + ssErrorsContent[PtrLinkFail]);

    setupPointers(*newServerPtr, *newMetaInfoPtr);

    pSubDialog = unique_ptr<MenuSubDialog>(new MenuSubDialog(this));

    pGameManagerWindow = unique_ptr<GameManagerWindow>(new GameManagerWindow(pServer(), pUserMetaInfo()));

    pLobbyWindow = unique_ptr<LobbyWindow>(new LobbyWindow(pServer(), pUserMetaInfo(), pGameManagerWindow, this));

    connect(pLobbyWindow.get(), &LobbyWindow::goToMenuWindow,
            this, &MenuWindow::show);

    connect(pLobbyWindow.get(), &LobbyWindow::initLogoutChain,
            this, &MenuWindow::supportLogoutChain);

    connect(&refreshDataTimer, &QTimer::timeout, this, &MenuWindow::windowDataRefresh);

    ui->setupUi(this);
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::windowDataRefresh()
{
    if(!isEnabled() || isHidden())
        return;

    if(!FileManager::checkUserMetaIntegrity())
    {
        logoutBackToLoginWindow();
        return;
    }

    ResponseFromServerComm<vector<LobbyShortInfo>&> response = pServer()->get()->getLobbiesShortInfo();

    if(response.responseFlag == AllGoodRf)
    {
        timedOutCounter = 0;
        setupLobbiesTable();
        return;
    }
    else if(response.responseFlag == TimedOutRf)
    {
        timedOutCounter++;
        checkTimedOutCounter();
        return;
    }
}

void MenuWindow::quitApp()
{
    QCoreApplication::quit();
}

void MenuWindow::changeAcc()
{
    if(makeDialog(BaseWin::ChangeAcc, "", this) == 0)
    {
        logoutBackToLoginWindow();
    }
}

void MenuWindow::apply3dDiceState()
{
    FileManager::apply3dDiceStateToLocal(ui->aDiceIf3D->isChecked());
}

void MenuWindow::lobbyClicked(QTableWidgetItem *itemClicked)
{
    selectedLobbyUniqueId = ui->tLobbies->item(ui->tLobbies->row(itemClicked), UNIQUE_ID_COL)->text().toInt();
    ui->statusbar->showMessage
                (
                    ui->tLobbies->item(ui->tLobbies->row(itemClicked),
                                       LOBBY_NAME_COL)->text() +
                                       ssStatusBarSubMessage +
                                       QString::number(selectedLobbyUniqueId), 0
                );
}

void MenuWindow::applyLobbyFilter(QString textChanged)
{
    tableClear(*ui->tLobbies);
    tableSetupFill(*ui->tLobbies, pServer()->get()->getStableLobbiesList(), textChanged);
}

void MenuWindow::joinToLobby(QTableWidgetItem *itemActivated)
{
    QTableWidgetItem &lobbyItem = *ui->tLobbies->selectedItems().at(0);
    lobbyClicked(&lobbyItem);
    int answer = makeDialog(BaseWin::JoinLobby, ui->tLobbies->item(ui->tLobbies->row(itemActivated),
                                                                   LOBBY_NAME_COL)->text(), this);

    if(answer != 0)
        return;

    switchJoinByItem(*ui->tLobbies->selectedItems().at(0));
}

void MenuWindow::joinToLobby()
{
    const QList<QTableWidgetItem*> selectedItems = ui->tLobbies->selectedItems();
    if(selectedItems.isEmpty())
    {
        execErrorBox(ssLobbyNotSelected, this);
        return;
    }
    lobbyClicked(selectedItems.at(0));

    switchJoinByItem(*selectedItems.at(0));
}

void MenuWindow::joinIdDialog()
{
    ResponseFromServerComm<LobbyFullInfo> response;

    pSubDialog.get()->selfConfig(MenuSubDialog::JoinById);
    if(pSubDialog.get()->exec() == QDialog::Accepted)
    {
        int enteredUniqueId = pSubDialog.get()->uniqueIdValue();

        response = pServer()->get()->connectToLobby(enteredUniqueId);

        if(response.responseFlag == LobbyHasPasswordRf)
        {
            pSubDialog->selfConfig(MenuSubDialog::LobbyPasswordEnter);
            if(pSubDialog.get()->exec() == QDialog::Accepted)
            {
                response = pServer()->get()->connectToLobby(enteredUniqueId,
                                                            pSubDialog.get()->lobbyPasswordValue());

                switch (response.responseFlag)
                {
                case AllGoodRf:
                    timedOutCounter = 0;
                    m_firstContext = response.payload;
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
                                      QString::fromStdString(ssErrorsContent[LobbyConnectFail])),
                                 this);
                    logoutBackToLoginWindow();
                    return;
                }
            }
            else
                return;
        }
        else
        {
            switch (response.responseFlag)
            {
            case AllGoodRf:
                timedOutCounter = 0;
                m_firstContext = response.payload;
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
                                  QString::fromStdString(ssErrorsContent[LobbyConnectFail])),
                             this);
                logoutBackToLoginWindow();
                return;
            }
        }
    }
    else
        return;
}

void MenuWindow::createLobby()
{
    ResponseFromServerComm<LobbyFullInfo> response;
    response = pServer()->get()->createLobby(FileManager::getLastSettingsFromLocal());

    switch (response.responseFlag)
    {
    case AllGoodRf:
        timedOutCounter = 0;
        m_firstContext = response.payload;
        showLobbyWindow();
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
                          QString::fromStdString(ssErrorsContent[LobbyCreateFail])),
                     this);
        logoutBackToLoginWindow();
        return;
    }
}

void MenuWindow::findRanked()
{
    ResponseFromServerComm<LobbyFullInfo> response;
    response = pServer()->get()->connectToRankedLobby();

    switch (response.responseFlag)
    {
    case AllGoodRf:
        timedOutCounter = 0;
        m_firstContext = response.payload;
        showLobbyWindow();
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
                          QString::fromStdString(ssErrorsContent[RankedSearchFail])),
                     this);
        logoutBackToLoginWindow();
        return;
    }
}

void MenuWindow::showAbout()
{
    QMessageBox qmb(QMessageBox::Information, ssAboutTitle, ssAboutBody, QMessageBox::NoButton, this);
    qmb.exec();
}

void MenuWindow::chooseNickname()
{
    pSubDialog.get()->selfConfig(MenuSubDialog::ChangeNickname);
    if(pSubDialog.get()->exec() == QDialog::Accepted)
    {
        QString gotNickname = pSubDialog.get()->nicknameValue();
        ui->lNickname->setText(gotNickname);
        pServer()->get()->changeNickname(gotNickname);
        HostUserData temp = pUserMetaInfo()->get()->getHostInfo();
        temp.nickname = gotNickname;
        pUserMetaInfo()->get()->setHostInfo(temp);
    }
}

void MenuWindow::checkStatusBarToClear()
{
    if(ui->tLobbies->selectedItems().isEmpty())
        ui->statusbar->clearMessage();
}

void MenuWindow::closeEvent(QCloseEvent *event)
{
    quitAppDialog();
    event->ignore();
}

void MenuWindow::quitAppDialog()
{
    if(makeDialog(BaseWin::QuitApp, "", this) == 0)
        QCoreApplication::quit();
}

void MenuWindow::show()
{
#ifdef DEBUG_FAST_LOAD_INTO_THE_GAME
    pGameManagerWindow->show();
    this->hide();
    return;
#endif
    setDisabled(true);
    timedOutCounter = 0;
    refreshDataTimer.stop();

    setupLobbiesFilter();

    ResponseFromServerComm<HostUserData> response = pServer()->get()->getCurrentHostInfo(false);

    switch (response.responseFlag)
    {
    case AllGoodRf:
    {
        timedOutCounter = 0;
        pUserMetaInfo()->get()->setHostInfo(response.payload);

        uint8_t host3dDicePreference = FileManager::getUser3dDicePreference();
        if(host3dDicePreference == FileManager::DicePrefNotStated)
            ui->aDiceIf3D->setChecked(true);
        else
            ui->aDiceIf3D->setChecked(host3dDicePreference);

        displayHostShortInfo();
        FileManager::apply3dDiceStateToLocal(ui->aDiceIf3D->isChecked());

        setDisabled(false);
        windowDataRefresh();
        refreshDataTimer.start(REFRESH_LOBBIES_LIST_EVERY_N_MS);
        QMainWindow::show();
        break;
    }
    case UnauthorizeRf:
        timedOutCounter = 0;
        logoutBackToLoginWindow();
        return;
    case TimedOutRf:
        logoutBackToLoginWindow();
        return;
    default:
        execErrorBox(QString("%1%2")
                     .arg(QString::fromStdString(ssClassNames[ServerCommCN]),
                          QString::fromStdString(ssErrorsContent[GetHostInfoFail])),
                     this);
        logoutBackToLoginWindow();
        return;
    }
}

void MenuWindow::hide()
{
    refreshDataTimer.stop();
    setDisabled(true);
    QMainWindow::hide();
}

void MenuWindow::supportLogoutChain()
{
    logoutBackToLoginWindow();
}

void MenuWindow::setupLobbiesTable()
{
    const std::vector<LobbyShortInfo>* pLobbiesVec = &pServer()->get()->getStableLobbiesList();

    tableClear(*ui->tLobbies);

    tableSetupFill(*ui->tLobbies, *pLobbiesVec, ui->leLobbyFilter->text());

    ui->lLobbiesCount->setText(ssLobbiesInTotal
                               + QString::number(pLobbiesVec->size())
                               + ssLobbiesNotPassworded
                               + QString::number(count_if(pLobbiesVec->begin(),
                                                          pLobbiesVec->end(),
                                                          [](const LobbyShortInfo &lobby)
                                                          { return !lobby.isPassworded; })));
}

void MenuWindow::setupLobbiesFilter()
{
    QRegularExpression lobbyFilterRegExp(ssRegExps[LobbyNameRegExp]);
    QRegularExpressionValidator* lobbyFilterValidator = new QRegularExpressionValidator(lobbyFilterRegExp, this);
    ui->leLobbyFilter->setValidator(lobbyFilterValidator);
    ui->leLobbyFilter->setMaxLength(LOBBY_NAME_MAX_LEN);
    ui->leLobbyFilter->clear();
}

void MenuWindow::displayHostShortInfo()
{
    HostUserData hostUserData = pUserMetaInfo()->get()->getHostInfo();
    ui->lNickname->setText(hostUserData.nickname);
    ui->lRpCount->setText(QString::number(hostUserData.rpCount) + " RP");

    ui->lSubVertBar->setHidden(hostUserData.isGuest);
    ui->lRpCount->setHidden(hostUserData.isGuest);

    ui->bRankedSearch->setDisabled(hostUserData.isGuest);
}

void MenuWindow::tableClear(QTableWidget &table)
{
    table.clearContents();
    table.clear();
    table.setColumnCount(0);
    table.setRowCount(0);
}

void MenuWindow::tableSetupFill(QTableWidget &table, const vector<LobbyShortInfo> &contentVec, const QString &filter)
{
    tableClear(table);

    const short tCols = LOBBIES_TABLE_COLS;
    const int tRows = count_if(contentVec.begin(),
                               contentVec.end(),
                               [filter](const LobbyShortInfo &lobby)
                               { return lobby.name.toLower().contains(filter.toLower()); });

    table.setColumnCount(tCols);
    table.setRowCount(tRows);

    table.hideColumn(UNIQUE_ID_COL);

    table.setHorizontalHeaderLabels(ssLobbyTableLabels);
    table.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = 0;
    for(auto &lsiItem : contentVec)
    {
        const QString uniqueId = QString::number(lsiItem.uniqueId);
        const QString lobbyName = lsiItem.name;
        if(!lobbyName.toLower().contains(filter.toLower()))
            continue;
        const QString isPassworded = lsiItem.isPassworded ? ssPassColumnYes : ssPassColumnNo;
        const QString playersCount = QString::number(lsiItem.curPlayersCount) + ssPlayersColumnSlash
                + QString::number(lsiItem.maxPlayersCount);
        QTableWidgetItem* items[] = {
                                        new QTableWidgetItem(lobbyName),
                                        new QTableWidgetItem(isPassworded),
                                        new QTableWidgetItem(playersCount),
                                        new QTableWidgetItem(uniqueId)
                                    };
        for(auto &i : items)
            i->setTextAlignment(Qt::AlignCenter);

        for(short int col = 0; col < tCols; col++)
            table.setItem(row, col, items[col]);

        row++;
    }

    table.sortByColumn(IS_PASSWORDED_COL, Qt::DescendingOrder);
}

void MenuWindow::switchJoinByItem(const QTableWidgetItem &item)
{
    switch (checkIfPassworded(item))
    {
    case DialogCodes::PassEntered:
    {
        ResponseFromServerComm<LobbyFullInfo> response;
        response = pServer()->get()->connectToLobby(selectedLobbyUniqueId,
                                                    pSubDialog.get()->lobbyPasswordValue());

        switch (response.responseFlag)
        {
        case AllGoodRf:
            timedOutCounter = 0;
            m_firstContext = response.payload;
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
                              QString::fromStdString(ssErrorsContent[LobbyConnectFail])),
                         this);
            logoutBackToLoginWindow();
            return;
        }
        break;
    }
    case DialogCodes::NoPassword:
    {
        ResponseFromServerComm<LobbyFullInfo> response;
        response = pServer()->get()->connectToLobby(selectedLobbyUniqueId);

        if(response.responseFlag == LobbyHasPasswordRf)
        {
            pSubDialog->selfConfig(MenuSubDialog::LobbyPasswordEnter);
            if(pSubDialog.get()->exec() == QDialog::Accepted)
            {
                response = pServer()->get()->connectToLobby(selectedLobbyUniqueId,
                                                            pSubDialog.get()->lobbyPasswordValue());

                switch (response.responseFlag)
                {
                case AllGoodRf:
                    timedOutCounter = 0;
                    m_firstContext = response.payload;
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
                                      QString::fromStdString(ssErrorsContent[LobbyConnectFail])),
                                 this);
                    logoutBackToLoginWindow();
                    return;
                }
            }
            else
                return;
        }
        else
        {
            switch (response.responseFlag)
            {
            case AllGoodRf:
                timedOutCounter = 0;
                m_firstContext = response.payload;
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
                                  QString::fromStdString(ssErrorsContent[LobbyConnectFail])),
                             this);
                logoutBackToLoginWindow();
                return;
            }
        }
        break;
    }
    default:
        return;
    }
}

void MenuWindow::showLobbyWindow()
{
    hide();
    pLobbyWindow.get()->show(m_firstContext);
}

void MenuWindow::logoutBackToLoginWindow()
{
    hide();
    pServer()->get()->clearTemporaryHostData();
    FileManager::clearUserMetaForNewLogin();
    emit switchToLoginWindow();
}

void MenuWindow::checkTimedOutCounter()
{
    if(timedOutCounter >= MAX_TIMED_OUT_COUNTER)
    {
        timedOutCounter = 0;
        logoutBackToLoginWindow();
    }
}

dialogCode MenuWindow::checkIfPassworded(const QTableWidgetItem &item)
{
    if(ui->tLobbies->item(item.row(), IS_PASSWORDED_COL)->text() == ssPassColumnYes)
    {
        pSubDialog.get()->selfConfig(MenuSubDialog::LobbyPasswordEnter,
                                     ui->tLobbies->item(item.row(), LOBBY_NAME_COL)->text());
        if(pSubDialog.get()->exec() == QDialog::Accepted)
        {
            return DialogCodes::PassEntered;
        }
        else return DialogCodes::PassRejected;
    }
    return DialogCodes::NoPassword;
}
