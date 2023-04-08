#include "menuwindow.h"
#include "ui_menuwindow.h"

MenuWindow::MenuWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                       unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuWindow)
{
    if(!*newServerPtr || !*newMetaInfoPtr)
        throw std::runtime_error(menuWindowClassName + errorPtrLink);

    setupPointers(*newServerPtr, *newMetaInfoPtr);

    ui->setupUi(this);

    pSubDialog = unique_ptr<LobbiesSubDialog>(new LobbiesSubDialog());

    this->setupLobbiesFilter();

    pLobbyWindow = unique_ptr<LobbyWindow>(new LobbyWindow(pServer(), pUserMetaInfo()));

    connect(pLobbyWindow.get(), &LobbyWindow::goToMenuWindow,
            this, &MenuWindow::showAndRefresh);
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::windowDataRefresh()
{
    pUserMetaInfo()->get()->setHostInfo(pServer()->get()->getCurrentHostInfo());
    ui->setupUi(this);
    this->setDisabled(true);
    this->setupLobbiesTable();
    this->displayHostShortInfo();
    this->apply3dDiceState();
    this->setEnabled(true);
}

void MenuWindow::quitApp()
{
    if(makeDialog(BaseWin::QuitApp) == 0)
        QCoreApplication::quit();
}

void MenuWindow::changeAcc()
{
    if(makeDialog(BaseWin::ChangeAcc) == 0)
    {
        this->hide();
        emit goToLoginWindow();
    }
}

void MenuWindow::apply3dDiceState()
{
    pUserMetaInfo()->get()->apply3dDiceChoise(this->ui->aDiceIf3D->isChecked());
}

void MenuWindow::lobbyClicked(QTableWidgetItem *itemClicked)
{
    curLobbyUniqueId = ui->tLobbies->item(ui->tLobbies->row(itemClicked), UNIQUE_ID_COL)->text().toInt();
    ui->statusbar->showMessage
                (
                    ui->tLobbies->item(ui->tLobbies->row(itemClicked),
                                       LOBBY_NAME_COL)->text() +
                                       statusBarSubMessage +
                                       QString::number(curLobbyUniqueId), 0
                );
}

void MenuWindow::applyLobbyFilter(QString textChanged)
{
    tableClear(*ui->tLobbies);
    tableSetupFill(*ui->tLobbies, pServer()->get()->getLobbiesShortInfo(), textChanged);
}

void MenuWindow::joinToLobby(QTableWidgetItem *itemActivated)
{
    QTableWidgetItem &lobbyItem = *ui->tLobbies->selectedItems().at(0);
    this->lobbyClicked(&lobbyItem);
    int answer = makeDialog(BaseWin::JoinLobby, ui->tLobbies->item(ui->tLobbies->row(itemActivated), LOBBY_NAME_COL)->text());

    if(answer != 0)
        return;

    switchJoinByItem(*ui->tLobbies->selectedItems().at(0));
}

void MenuWindow::joinToLobby()
{
    const QList<QTableWidgetItem*> selectedItems = ui->tLobbies->selectedItems();
    if(selectedItems.isEmpty())
    {
        this->execErrorBox(lobbyNotSelected);
        return;
    }
    this->lobbyClicked(selectedItems.at(0));

    switchJoinByItem(*selectedItems.at(0));
}

void MenuWindow::joinIdDialog()
{
    pSubDialog.get()->selfConfig(LobbiesSubDialog::JoinById);
    if(pSubDialog.get()->exec() == QDialog::Accepted)
    {
        // !!! STUB !!! NEED TO CHECK IF PASSWORDED!
        try
        {
            m_firstContext = pServer()->get()->tryJoinById(pSubDialog.get()->uniqueIdValue());
        }
        catch (std::exception &e)
        {
            this->execErrorBox(e.what());
            return;
        }
    }
    showLobbyWindow();
}

void MenuWindow::createLobby()
{
    try
    {
        m_firstContext = pServer()->get()->tryCreateLobby(pUserMetaInfo()->get()->getHostInfo().uniqueUserId);
    }
    catch (std::exception &e)
    {
        this->execErrorBox(e.what());
        return;
    }
    showLobbyWindow();
}

void MenuWindow::findRanked()
{
    try
    {
        m_firstContext = pServer()->get()->tryRankedQueue(pUserMetaInfo()->get()->getHostInfo().uniqueUserId);
    }
    catch (std::exception &e)
    {
        this->execErrorBox(e.what());
        return;
    }
    showLobbyWindow();
}

void MenuWindow::showAbout()
{
    QMessageBox qmb(QMessageBox::Information, aboutTitle, aboutBody);
    qmb.exec();
}

void MenuWindow::chooseNickname()
{
    pSubDialog.get()->selfConfig(LobbiesSubDialog::ChangeNickname);
    if(pSubDialog.get()->exec() == QDialog::Accepted)
    {
        try
        {
            pServer()->get()->tryChangeNickname(pSubDialog.get()->nicknameValue());
        }
        catch (std::exception &e)
        {
            this->execErrorBox(e.what());
            return;
        }
    }
}

void MenuWindow::showAndRefresh()
{
    this->windowDataRefresh();
    this->show();
}

void MenuWindow::setupLobbiesTable()
{
    const std::vector<LobbyShortInfo>* pLobbiesVec = &pServer()->get()->getLobbiesShortInfo();

    this->tableClear(*ui->tLobbies);

    this->tableSetupFill(*ui->tLobbies, *pLobbiesVec, ui->leLobbyFilter->text());

    ui->lLobbiesCount->setText(lobbiesInTotal
                               + QString::number(pLobbiesVec->size())
                               + lobbiesNotPassworded
                               + QString::number(count_if(pLobbiesVec->begin(),
                                                          pLobbiesVec->end(),
                                                          [](const LobbyShortInfo &lobby)
                                                          { return !lobby.isPassworded; })));
}

void MenuWindow::setupLobbiesFilter()
{
    QRegularExpression lobbyFilterRegExp(lobbyRegExpString);
    QRegularExpressionValidator* lobbyFilterValidator = new QRegularExpressionValidator(lobbyFilterRegExp, this);
    ui->leLobbyFilter->setValidator(lobbyFilterValidator);
    ui->leLobbyFilter->clear();
}

void MenuWindow::displayHostShortInfo()
{
    ui->lNickname->setText(pUserMetaInfo()->get()->getHostInfo().userName);
    ui->lRpCount->setText(QString::number(pUserMetaInfo()->get()->getHostInfo().userRpCount) + " RP");
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
    const short int tCols = LOBBIES_TABLE_COLS;
    const int tRows = count_if(contentVec.begin(),
                               contentVec.end(),
                               [filter](const LobbyShortInfo &lobby)
                               { return lobby.lobbyName.toLower().contains(filter.toLower()); });

    table.setColumnCount(tCols);
    table.setRowCount(tRows);

    table.hideColumn(UNIQUE_ID_COL);

    table.setHorizontalHeaderLabels(lobbyTableLabels);
    table.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = 0;
    for(auto &lsiItem : contentVec)
    {
        const int uniqueId = lsiItem.uniqueId;
        const QString lobbyName = lsiItem.lobbyName;
        if(!lobbyName.toLower().contains(filter.toLower()))
            continue;
        const QString isPassworded = lsiItem.isPassworded ? passColumnYes : passColumnNo;
        const QString playersCount = QString::number(lsiItem.curPlayersCount) + playersColumnSlash
                + QString::number(lsiItem.maxPlayersCount);
        QTableWidgetItem* items[] = {
                                        new QTableWidgetItem(lobbyName),
                                        new QTableWidgetItem(isPassworded),
                                        new QTableWidgetItem(playersCount),
                                        new QTableWidgetItem(QString::number(uniqueId))
                                    };
        for(auto &i : items)
            i->setTextAlignment(Qt::AlignCenter);

        for(short int col = 0; col < tCols; col++)
        {
            table.setItem(row, col, items[col]);
        }
        row++;
    }

    table.sortByColumn(IS_PASSWORDED_COL, Qt::DescendingOrder);
}

void MenuWindow::switchJoinByItem(const QTableWidgetItem &item)
{
    switch (this->checkIfPassworded(item))
    {
    case DialogCodes::PassEntered:
        try
        {
            m_firstContext = pServer()->get()->tryJoinById(curLobbyUniqueId, pSubDialog.get()->lobbyPasswordValue());
        }
        catch (std::exception &e)
        {
            this->execErrorBox(e.what());
            return;
        }
        break;
    case DialogCodes::NoPassword:
        try
        {
            m_firstContext = pServer()->get()->tryJoinById(curLobbyUniqueId);
        }
        catch (std::exception &e)
        {
            this->execErrorBox(e.what());
            return;
        }
        break;
    default:
        return;
    }
    showLobbyWindow();
}

void MenuWindow::showLobbyWindow()
{
    this->hide();
    pLobbyWindow.get()->giveFirstContext(m_firstContext);
    pLobbyWindow.get()->showAndRefresh();
}

dialogCode MenuWindow::checkIfPassworded(const QTableWidgetItem &item)
{
    if(ui->tLobbies->item(item.row(), IS_PASSWORDED_COL)->text() == passColumnYes)
    {
        pSubDialog.get()->selfConfig(LobbiesSubDialog::LobbyPasswordEnter,
                                     ui->tLobbies->item(item.row(), LOBBY_NAME_COL)->text());
        if(pSubDialog.get()->exec() == QDialog::Accepted)
        {
            return DialogCodes::PassEntered;
        }
        else return DialogCodes::PassRejected;
    }
    return DialogCodes::NoPassword;
}
