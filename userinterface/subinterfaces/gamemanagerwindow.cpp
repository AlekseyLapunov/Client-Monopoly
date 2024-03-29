#include "gamemanagerwindow.h"
#include "ui_gamemanagerwindow.h"

GameManagerWindow::GameManagerWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                                     unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                                     QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameManagerWindow)
{
    if(!*newServerPtr || !*newMetaInfoPtr)
        throw std::runtime_error(ssClassNames[GameManagerCN] + ssErrorsContent[PtrLinkFail]);

    setupPointers(*newServerPtr, *newMetaInfoPtr);

    gameTransmitterObj = new GameTransmitterObject(this);

    qmlEngine = nullptr;

    ui->setupUi(this);
}

GameManagerWindow::~GameManagerWindow()
{
    gameReceiverObj->deleteLater();
    qmlEngine->deleteLater();
    gameTransmitterObj->deleteLater();
    gameReceiverObj = nullptr;
    delete ui;
}

void GameManagerWindow::show()
{
#ifdef DEBUG_SHOW_DEBUG_PANEL
    QWidget::show();
#else
    QWidget::hide();
    return;
#endif
    startQmlEngine();
    reactToPlayerNumberComboBoxChange();
}

void GameManagerWindow::closeEvent(QCloseEvent *event)
{
    QCoreApplication::quit();
    event->ignore();
}

void GameManagerWindow::startQmlEngine()
{
    if(qmlEngine != nullptr)
    {
        qDebug().noquote() << "QML Engine: Already created. Reseting...";
        qmlEngine->clearComponentCache();
        qmlEngine->clearSingletons();
        delete qmlEngine;
        qmlEngine = nullptr;
        delete gameTransmitterObj;
        gameTransmitterObj = new GameTransmitterObject(this);
        gameReceiverObj = nullptr;
        qDebug().noquote() << "QML Engine: Reseting done successfully.";
    }

    qmlRegisterSingletonType(QUrl("qrc:/qmlfiles/HelperSingletone/Helper.qml"), "Helper", 1, 0, "Helper");
    qmlRegisterType<FieldsGridModel>("GameMap", 1, 0, "FieldsGridModel");
    qmlRegisterUncreatableType<CellsList>("GameMap", 1, 0, "CellsList",
                                                QStringLiteral("Should not be created inside QML"));
    qmlRegisterType<PlayersInfoModel>("GamePlayers", 1, 0, "PlayersInfoModel");
    qmlRegisterUncreatableType<PlayerGameInfoList>("GamePlayers", 1, 0, "PlayerGameInfoList",
                                                    QStringLiteral("Should not be created inside QML"));

    qmlEngine = new QQmlApplicationEngine;

    m_cellsList = new CellsList();
    m_playersList = new PlayerGameInfoList();
    applyFirstGameContext();

    qmlEngine->rootContext()->setContextProperty(QStringLiteral("_cellsList"), m_cellsList);
    qmlEngine->rootContext()->setContextProperty(QStringLiteral("_playersList"), m_playersList);
    qmlEngine->rootContext()->setContextProperty("_gameTransmitter", gameTransmitterObj);

    qmlEngine->load(QUrl("qrc:/qmlfiles/GameWindow.qml"));

    emit gameTransmitterObj->setHostPlayerNumber(hostPlayerNumber);
    passOwningObjectsToQml();

    gameReceiverObj = qmlEngine->rootObjects().at(0);
    QObject::connect(gameReceiverObj, SIGNAL(qmlGameWindowClosed()),
                     this, SLOT(manageQmlWindowClosing()));
    QObject::connect(gameReceiverObj, SIGNAL(sayBuyClicked()),
                     this, SLOT(manageBuyClicked()));
    QObject::connect(gameReceiverObj, SIGNAL(sabotageResult(int)),
                     this, SLOT(manageSabotageResult(int)));
    QObject::connect(gameReceiverObj, SIGNAL(arrowResolveResult(int, int)),
                     this, SLOT(manageArrowResolveResult(int, int)));
}

void GameManagerWindow::setStage()
{
    if(ui->sbStageNumber->value() != currentStage)
    {
        emit gameTransmitterObj->setCurrentGameStage(ui->sbStageNumber->value(),
                                                     !ui->chbNoStageAnimation->isChecked());
        currentStage = ui->sbStageNumber->value();
    }
}

void GameManagerWindow::changePlayerBalance()
{
    uint8_t playerNumberSelected = ui->cbPlayerNumber->currentIndex() + 1;

    if(playerNumberSelected > (uint8_t)m_playersList->items().size())
        return;

    int index = m_playersList->findIndexByPlayerNumber(playerNumberSelected);
    PlayerGameInfo playerGameInfo = m_playersList->getItemAt(index);
    playerGameInfo.currentBalance = ui->sbPlayerMoney->value();
    m_playersList->setItemAt(index, playerGameInfo);

    emit gameTransmitterObj->playerBalanceUpdate(playerNumberSelected,
                                                 ui->sbPlayerMoney->value());

    m_playersList->sortByBalance();
}

void GameManagerWindow::changePlayerNickname()
{
    uint8_t playerNumberSelected = ui->cbPlayerNumber->currentIndex() + 1;

    if(playerNumberSelected > (uint8_t)m_playersList->items().size())
        return;

    int index = m_playersList->findIndexByPlayerNumber(playerNumberSelected);
    PlayerGameInfo playerGameInfo = m_playersList->getItemAt(index);
    playerGameInfo.displayableName = ui->lePlayerNickname->text();

    m_playersList->setItemAt(index, playerGameInfo);

    emit gameTransmitterObj->playerNicknameUpdate(playerNumberSelected,
                                                  ui->lePlayerNickname->text());
}

void GameManagerWindow::rollDiceDirectional()
{
    emit gameTransmitterObj->startDiceDirectionalAnimation(ui->sbLeftDiceValue->value(),
                                                           ui->sbRightDiceValue->value());
}

void GameManagerWindow::rollDiceInfinite()
{
    emit gameTransmitterObj->startDiceInfiniteAnimation();
}

void GameManagerWindow::addPlayer()
{
    if(m_playersList->items().size() >= 6)
        return;

    int settingBalance = m_playersList->getItemAt(0).currentBalance + 20;

    PlayerGameInfo item = {static_cast<uint8_t>(m_playersList->items().size() + 1),
                           QString("Player%1").arg(m_playersList->items().size() + 1),
                           settingBalance, 1};

    emit gameTransmitterObj->appendPlayer(item.playerNumber,
                                          item.displayableName,
                                          item.currentBalance,
                                          item.piecePositionOnOrderIndex);

    changePiecesMaskByOrderIndex(item.piecePositionOnOrderIndex,
                                 item.playerNumber,
                                 ChangingPiecesMask::AddPM);

    m_playersList->sortByBalance();
    reactToPlayerNumberComboBoxChange();
}

void GameManagerWindow::removePlayer()
{
    if(m_playersList->items().size() <= 2)
        return;

    m_playersList->sortByBalance();

    PlayerGameInfo playerToDelete = m_playersList->getItemAt(
                m_playersList->findIndexByPlayerNumber(static_cast<uint8_t>(m_playersList->items().size()))
                );

    if(checkIfOrderIndexIsValid(playerToDelete.piecePositionOnOrderIndex))
    {
        changePiecesMaskByOrderIndex(playerToDelete.piecePositionOnOrderIndex,
                                     playerToDelete.playerNumber,
                                     ChangingPiecesMask::DeletePM);
    }

    emit gameTransmitterObj->deletePlayer(static_cast<uint8_t>(m_playersList->items().size()));
    reactToPlayerNumberComboBoxChange();
}

void GameManagerWindow::placePlayerPieceOn()
{
    uint8_t chosenPlayerNumber = ui->cbPlayerNumber->currentIndex() + 1;

    if(chosenPlayerNumber > m_playersList->items().size())
        return;

    int chosenOrderIndex = ui->sbFieldIndex->value();

    if(!checkIfOrderIndexIsValid(chosenOrderIndex))
        return;

    int currentOrderIndex = m_playersList->getItemAt
            (
                m_playersList->findIndexByPlayerNumber(chosenPlayerNumber)
            ).piecePositionOnOrderIndex;

    if(checkIfOrderIndexIsValid(currentOrderIndex))
    {
        changePiecesMaskByOrderIndex(currentOrderIndex,
                                     chosenPlayerNumber,
                                     ChangingPiecesMask::DeletePM);
    }

    changePiecesMaskByOrderIndex(chosenOrderIndex,
                                 chosenPlayerNumber,
                                 ChangingPiecesMask::AddPM);

    uint8_t playerIndex = m_playersList->findIndexByPlayerNumber(chosenPlayerNumber);
    PlayerGameInfo player = m_playersList->getItemAt(playerIndex);
    player.piecePositionOnOrderIndex = chosenOrderIndex;

    m_playersList->setItemAt(playerIndex, player);

    emit gameTransmitterObj->playerPiecePositionUpdate(player.playerNumber,
                                                       player.piecePositionOnOrderIndex);
}

void GameManagerWindow::endTheGame()
{
    m_playersList->sortByBalance();
    emit gameTransmitterObj->finishTheGame();
}

void GameManagerWindow::reactToPlayerNumberComboBoxChange()
{ 
    uint8_t playerNumberSelected = ui->cbPlayerNumber->currentIndex() + 1;

    if(playerNumberSelected > (uint8_t)m_playersList->items().size())
    {
        ui->lePlayerNickname->setText("");
        ui->sbPlayerMoney->setValue(ui->sbPlayerMoney->minimum());
        return;
    };

    uint8_t index = (uint8_t)m_playersList->findIndexByPlayerNumber(playerNumberSelected);

    ui->lePlayerNickname->setText(m_playersList->getItemAt(index).displayableName);
    ui->sbPlayerMoney->setValue(m_playersList->getItemAt(index).currentBalance);
}

void GameManagerWindow::allowAction(bool allowActionFlag)
{
    emit gameTransmitterObj->manageActionMode(allowActionFlag);
}

void GameManagerWindow::showTraceAlgo()
{
    uint8_t chosenPlayerNumber = ui->cbPlayerNumber->currentIndex() + 1;

    if(chosenPlayerNumber > m_playersList->items().size())
        return;

    vector<int> realCellsIndexes = getValuesFromPteAsRealIndexes();

    if(realCellsIndexes.size() < 2)
        return;

    if(m_playersList->getItemAt(m_playersList->findIndexByPlayerNumber(chosenPlayerNumber))
            .piecePositionOnOrderIndex != m_cellsList->getItemAt(realCellsIndexes.at(0)).orderIndex)
    {
        ui->cbPlayerNumber->setCurrentIndex(chosenPlayerNumber + 1);
        ui->sbFieldIndex->setValue(m_cellsList->getItemAt(realCellsIndexes.at(0)).orderIndex);
        placePlayerPieceOn();
    }

    vector<int> trace = GameAlgorithm::makeFullPath(realCellsIndexes);

    QEventLoop eventLoop;
    QTimer movePieceTimer;
    connect(&movePieceTimer, &QTimer::timeout,
            &eventLoop, &QEventLoop::quit);
    movePieceTimer.setSingleShot(true);

    for(int i = 0; i < (int)trace.size(); i++)
    {
        ui->cbPlayerNumber->setCurrentIndex(chosenPlayerNumber - 1);
        ui->sbFieldIndex->setValue(m_cellsList->getItemAt(trace.at(i)).orderIndex);
        placePlayerPieceOn();
        movePieceTimer.start(ui->sbTraceSpaceTime->value());
        eventLoop.exec();
        if(i == ((int)trace.size() - 1))
        {
            movePieceTimer.stop();
            eventLoop.quit();
        }
    }
}

void GameManagerWindow::makePlayerHost()
{
    uint8_t chosenPlayerNumber = ui->cbPlayerNumber->currentIndex() + 1;

    if(m_playersList->findIndexByPlayerNumber(chosenPlayerNumber) == -1)
        return;

    hostPlayerNumber = chosenPlayerNumber;

    emit gameTransmitterObj->setHostPlayerNumber(hostPlayerNumber);
    passOwningObjectsToQml();
}

void GameManagerWindow::showArrowResolve()
{
    emit gameTransmitterObj->arrowResolve(ui->cbArrowOrderIndex->currentText().toInt(), 25);
}

void GameManagerWindow::manageBuyClicked()
{
    qDebug().noquote() << "Caught \"buy clicked\"";
}

void GameManagerWindow::manageSabotageResult(int chosenOrderIndex)
{
    qDebug().noquote() << QString("Caught \"sabotage\" on Cell #%1")
                          .arg(QString::number(chosenOrderIndex));
}

void GameManagerWindow::manageArrowResolveResult(int inputOrderIndex, int chosenRotation)
{
    qDebug().noquote() << QString("%1 rotation on Arrow (Cell#%2): %3")
                          .arg(((chosenRotation != -1) ? "Accepted" : "Rejected"),
                               (QString::number(inputOrderIndex)),
                               (chosenRotation == ArrowDirection::ArrowUp    ? "ArrowUp"
                              : chosenRotation == ArrowDirection::ArrowRight ? "ArrowRight"
                              : chosenRotation == ArrowDirection::ArrowLeft  ? "ArrowLeft"
                              : chosenRotation == ArrowDirection::ArrowDown  ? "ArrowDown"
                              : "Denied"));
}

void GameManagerWindow::applyFirstGameContext()
{
    fillDebugMapContext();
    fillDebugPlayersContext();

    m_cellsList->setItems(debugMapContext);
    m_playersList->setItems(debugPlayersContext);
    m_playersList->sortByBalance();
}

void GameManagerWindow::initializeHostOwningObjectCounts()
{
    hostPlayerIncome = 0;
    for(uint8_t i = FieldType::Sawmill; i <= FieldType::Uranium; i++)
    {
        hostOwningObjectsCounts.insert(i, 0);
    }
}

void GameManagerWindow::countHostOwningObjectsByMap()
{
    hostPlayerIncome = 0;
    resetHostOwningObjectCounts();
    for(int i = 0; i < (int)m_cellsList->items().size(); i++)
    {
        Cell field = m_cellsList->getItemAt(i);

        if(field.fieldTypeSet < FieldType::Sawmill || field.fieldTypeSet > FieldType::Uranium)
            continue;

        if(field.playerNumberOwner != hostPlayerNumber)
            continue;

        hostOwningObjectsCounts[field.fieldTypeSet] = hostOwningObjectsCounts[field.fieldTypeSet] + 1;
        hostPlayerIncome += field.fieldIncome;
    }
}

void GameManagerWindow::resetHostOwningObjectCounts()
{
    hostOwningObjectsCounts.clear();
    initializeHostOwningObjectCounts();
}

void GameManagerWindow::passOwningObjectsToQml()
{
    countHostOwningObjectsByMap();
    emit gameTransmitterObj->hostOwningObjectsClear();
    for(uint8_t i = FieldType::Sawmill; i <= FieldType::Uranium; i++)
    {
        emit gameTransmitterObj->hostOwningObjectsUpdate(i, hostOwningObjectsCounts[i]);
    }
    emit gameTransmitterObj->hostIncomeUpdate(hostPlayerIncome);
}

bool GameManagerWindow::checkIfOrderIndexIsValid(int inputOrderIndex)
{
    return ((inputOrderIndex >= 1) && (inputOrderIndex <= 45));
}

void GameManagerWindow::changePiecesMaskByOrderIndex(int inputOrderIndex,
                                                     uint8_t whatPlayerNumber,
                                                     uint8_t addOrRemove)
{
    int foundIndex = m_cellsList->findIndexByOrderIndex(inputOrderIndex);

    uint8_t bit = (1 << (whatPlayerNumber - 1));

    Cell foundCell = m_cellsList->getItemAt(foundIndex);

    switch (addOrRemove)
    {
    case ChangingPiecesMask::DeletePM:
        foundCell.piecesOnCellMask &= (~bit);
        break;
    case ChangingPiecesMask::AddPM:
        foundCell.piecesOnCellMask |= bit;
        break;
    default:
        return;
    }

    m_cellsList->setItemAt(foundIndex, foundCell);
    emit gameTransmitterObj->fieldPiecesMaskUpdate(foundIndex, foundCell.piecesOnCellMask);
}

vector<int> GameManagerWindow::getValuesFromPteAsRealIndexes()
{
    QString text = ui->pteTraceInput->toPlainText();

    text.remove(" ");

    QStringList textToNumbers = text.split(",");

    vector<int> realIndexes = {};

    for (const QString& item : textToNumbers)
    {
        int orderIndex = item.toInt();
        if(!checkIfOrderIndexIsValid(orderIndex))
            continue;
        realIndexes.push_back(m_cellsList->findIndexByOrderIndex(orderIndex));
    }

    if(realIndexes.size() < 2)
        return {};

    return realIndexes;
}

void GameManagerWindow::checkTimedOutCounter()
{

}

void GameManagerWindow::manageQmlWindowClosing()
{
    qDebug().noquote() << "QML Window closed. Heading to MenuWindow";
#ifdef DEBUG_SHOW_DEBUG_PANEL
    QCoreApplication::quit();
#else
    emit goToMenuWindow();
    close();
#endif
}

void fillDebugMapContext()
{
    for(int i = 0; i < 81; i++)
        debugMapContext.appendItem(debugCellsArray[i]);
}

void fillDebugPlayersContext()
{
    for(int i = 0; i < 2; i++)
        debugPlayersContext.appendItem(debugPlayersArray[i].playerNumber,
                                       debugPlayersArray[i].displayableName,
                                       debugPlayersArray[i].currentBalance,
                                       debugPlayersArray[i].piecePositionOnOrderIndex);
}
