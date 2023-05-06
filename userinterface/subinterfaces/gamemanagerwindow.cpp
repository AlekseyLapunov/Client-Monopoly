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
    qmlRegisterUncreatableType<FieldsGridModel>("GameMap", 1, 0, "CellsList",
                                                QStringLiteral("Should NOT be created inside QML"));

    qmlEngine = new QQmlApplicationEngine;

    m_cellsList = new CellsList();
    applyFirstGameContext();

    qmlEngine->rootContext()->setContextProperty(QStringLiteral("_cellsList"), m_cellsList);
    qmlEngine->rootContext()->setContextProperty("_gameTransmitter", gameTransmitterObj);

    qmlEngine->load(QUrl("qrc:/qmlfiles/GameWindow.qml"));

    gameReceiverObj = qmlEngine->rootObjects().at(0);
    QObject::connect(gameReceiverObj, SIGNAL(qmlGameWindowClosed()),
                     this, SLOT(manageQmlWindowClosing()));
}

void GameManagerWindow::switchToStage1()
{
    emit gameTransmitterObj->startStageAnimation(1);
}

void GameManagerWindow::switchToStage2()
{
    emit gameTransmitterObj->startStageAnimation(2);
}

void GameManagerWindow::switchToStage3()
{
    emit gameTransmitterObj->startStageAnimation(3);
}

void GameManagerWindow::changePlayerBalance()
{

}

void GameManagerWindow::changePlayerNickname()
{

}

void GameManagerWindow::rollDiceDirectional()
{

}

void GameManagerWindow::rollDiceInfinite()
{

}

void GameManagerWindow::addPlayer()
{

}

void GameManagerWindow::removePlayer()
{

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
    //fillDebugPlayersContext();

    m_cellsList->setItems(debugMapContext);
    //m_playersList->setItems(debugPlayersContext);
}
