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

    gameReceiverObj = qmlEngine->rootObjects().at(0);
    QObject::connect(gameReceiverObj, SIGNAL(qmlGameWindowClosed()),
                     this, SLOT(manageQmlWindowClosing()));
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

}

void GameManagerWindow::changePlayerNickname()
{

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

    PlayerGameInfo item = {static_cast<uint8_t>(m_playersList->items().size() + 1),
                           QString("Player%1").arg(m_playersList->items().size() + 1),
                           m_playersList->getItemAt(0).currentBalance + 20, 0};
    m_playersList->appendItem(item);
    m_playersList->sortByBalance();

    emit gameTransmitterObj->appendPlayer(item.playerNumber,
                                          item.displayableName,
                                          item.currentBalance);
}

void GameManagerWindow::removePlayer()
{
    if(m_playersList->items().size() <= 2)
        return;

    m_playersList->removeItem(0);
    m_playersList->sortByBalance();

    emit gameTransmitterObj->deletePlayer(m_playersList->items().size() + 1);
}
void GameManagerWindow::placePlayerPieceOn()
{

}

void GameManagerWindow::endTheGame()
{

}

void GameManagerWindow::fakePlayerTurn()
{

}

void GameManagerWindow::reactToPlayerNumberSpinBoxChange()
{
    //ui->lePlayerNickname;
    //ui->sbPlayerMoney;
}

void GameManagerWindow::manageQmlWindowClosing()
{
    qDebug().noquote() << "QML Window closed. Quiting application";
    QCoreApplication::quit();
}

void GameManagerWindow::applyFirstGameContext()
{
    fillDebugMapContext();
    fillDebugPlayersContext();

    m_cellsList->setItems(debugMapContext);
    m_playersList->setItems(debugPlayersContext);
    m_playersList->sortByBalance();
}
