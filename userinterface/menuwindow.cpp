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
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::windowDataRefresh()
{
    this->setButtonsState(false);
    this->setupLobbiesTable();
    this->displayHostShortInfo();
    this->apply3dDiceState();
    this->setButtonsState(true);
}

void MenuWindow::quitApp()
{
    if(makeDialog(BaseWin::quitApp) == 0)
        QCoreApplication::quit();
}

void MenuWindow::changeAcc()
{
    if(makeDialog(BaseWin::changeAcc) == 0)
    {
        this->hide();
        emit goToLoginWindow();
    }
}

void MenuWindow::apply3dDiceState()
{
    HostUserData tempHostUserData = pUserMetaInfo()->get()->getHostInfo();
    tempHostUserData.uses3dDices = ui->aDiceIf3D->isChecked();
    pUserMetaInfo()->get()->setHostInfo(tempHostUserData);
}

void MenuWindow::lobbyClicked(QTableWidgetItem *itemClicked)
{
    curUniqueId = ui->tLobbies->item(ui->tLobbies->row(itemClicked), UNIQUE_ID_COL)->text().toInt();
    ui->statusbar->showMessage
                (
                    ui->tLobbies->item(ui->tLobbies->row(itemClicked), LOBBY_NAME_COL)->text()
                    + statusBarSubMessage +
                    QString::number(curUniqueId),
                    0
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
    int answer = makeDialog(BaseWin::joinLobby, ui->tLobbies->item(ui->tLobbies->row(itemActivated), LOBBY_NAME_COL)->text());

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
    pSubDialog.get()->selfConfig(LobbiesSubDialog::joinById);
    if(pSubDialog.get()->exec() == QDialog::Accepted)
    {
        // !!! STUB !!!
        pServer()->get()->tryJoinById(pSubDialog.get()->uniqueIdValue());
    }
}

void MenuWindow::showAbout()
{
    QMessageBox qmb(QMessageBox::Information, aboutTitle, aboutBody);
    qmb.exec();
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
    ui->lRpCount->setText("| " + QString::number(pUserMetaInfo()->get()->getHostInfo().userRpCount) + " RP");
}

void MenuWindow::setButtonsState(const bool makeEnabled)
{
    ui->bConnect->setEnabled(makeEnabled);
    ui->bCreateLobby->setEnabled(makeEnabled);
    ui->bRankedSearch->setEnabled(makeEnabled);
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
            pServer()->get()->tryJoinById(curUniqueId, pSubDialog.get()->lobbyPasswordValue());
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
            pServer()->get()->tryJoinById(curUniqueId);
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

}

dialogCode MenuWindow::checkIfPassworded(const QTableWidgetItem &item)
{
    if(ui->tLobbies->item(item.row(), IS_PASSWORDED_COL)->text() == passColumnYes)
    {
        pSubDialog.get()->selfConfig(LobbiesSubDialog::lobbyPasswordEnter,
                                     ui->tLobbies->item(item.row(), LOBBY_NAME_COL)->text());
        if(pSubDialog.get()->exec() == QDialog::Accepted)
        {
            return DialogCodes::PassEntered;
        }
        else return DialogCodes::PassRejected;
    }
    return DialogCodes::NoPassword;
}
