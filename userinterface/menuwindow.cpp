#include "menuwindow.h"
#include "ui_menuwindow.h"

MenuWindow::MenuWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                       unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuWindow)
{
    if(!*newServerPtr || !*newMetaInfoPtr)
        throw std::runtime_error("MenuWindow: helpers pointers are nullptr");

    setupPointers(*newServerPtr, *newMetaInfoPtr);

    ui->setupUi(this);
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::windowDataRefresh()
{
    this->setButtonsState(false);
    this->setupLobbiesTable();
    this->setupLobbiesFilter();
    this->setupHostShortInfo();
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
    ui->statusbar->showMessage
                (
                    "Выбрано лобби: " +
                    ui->tLobbies->item(ui->tLobbies->row(itemClicked), LOBBY_NAME_COL)->text()
                    + " | ID для подключения: " +
                    ui->tLobbies->item(ui->tLobbies->row(itemClicked), UNIQUE_ID_COL)->text(),
                    0
                );
}

void MenuWindow::setupLobbiesTable()
{
    const std::vector<LobbyShortInfo>* pLobbiesVec = &pServer()->get()->getLobbiesShortInfo();

    this->tableClear(*ui->tLobbies);

    this->tableSetupRowsCols(*ui->tLobbies, *pLobbiesVec);

    ui->lLobbiesCount->setText("Доступно лобби: " + QString::number(pLobbiesVec->size()));
}

void MenuWindow::setupLobbiesFilter()
{
    QRegularExpression lobbyFilterRegExp("^[a-zA-Zа-яА-Я0-9]+ ?[a-zA-Zа-яА-Я0-9]*$");
    QRegularExpressionValidator *lobbyFilterValidator = new QRegularExpressionValidator(lobbyFilterRegExp, this);
    ui->leLobbyFilter->setValidator(lobbyFilterValidator);
    ui->leLobbyFilter->clear();
}

void MenuWindow::setupHostShortInfo()
{
    ui->lNickname->setText(pUserMetaInfo()->get()->getHostInfo().userName);
    ui->lRpCount->setText("| " + QString::number(pUserMetaInfo()->get()->getHostInfo().userRpCount) + " RP");
}

void MenuWindow::setButtonsState(bool areToBeAccessible)
{
    ui->bConnect->setEnabled(areToBeAccessible);
    ui->bCreateLobby->setEnabled(areToBeAccessible);
    ui->bRankedSearch->setEnabled(areToBeAccessible);
}

void MenuWindow::tableClear(QTableWidget &table)
{
    table.clearContents();
    table.clear();
    table.setColumnCount(0);
    table.setRowCount(0);
}

void MenuWindow::tableSetupRowsCols(QTableWidget &table, const vector<LobbyShortInfo> &contentVec)
{
    const short int tCols = LOBBIES_TABLE_COLS;
    const int tRows = contentVec.size();

    table.setColumnCount(tCols);
    table.setRowCount(tRows);

    table.hideColumn(UNIQUE_ID_COL);

    table.setHorizontalHeaderLabels(lobbyTableLabels);
    table.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for(int row = 0; row < tRows; row++)
    {
        const int uniqueId = contentVec.at(row).uniqueId;
        const QString lobbyName = contentVec.at(row).lobbyName;
        const QString isPassworded = contentVec.at(row).isPassworded ? "Есть" : "Нет";
        const QString playersCount = QString::number(contentVec.at(row).curPlayersCount) + " / "
                + QString::number(contentVec.at(row).maxPlayersCount);
        QTableWidgetItem* items[] = {
                                            new QTableWidgetItem(lobbyName),
                                            new QTableWidgetItem(isPassworded),
                                            new QTableWidgetItem(playersCount),
                                            new QTableWidgetItem(QString::number(uniqueId))
                                         };
        for(auto i : items)
            i->setTextAlignment(Qt::AlignCenter);

        for(short int col = 0; col < tCols; col++)
        {
            table.setItem(row, col, items[col]);
        }
    }

    table.sortByColumn(IS_PASSWORDED_COL, Qt::DescendingOrder);
}
