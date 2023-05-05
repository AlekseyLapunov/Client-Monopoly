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

    closureTimer = new QTimer(this);

    ui->setupUi(this);
}

GameManagerWindow::~GameManagerWindow()
{
    rootObj->deleteLater();
    qmlEngine->deleteLater();
    closureTimer->deleteLater();
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

    qmlEngine->load(QUrl("qrc:/qmlfiles/GameWindow.qml"));

    rootObj = qmlEngine->rootObjects().at(0);
    QObject::connect(rootObj, SIGNAL(qmlGameWindowClosed()),
                     this, SLOT(manageQmlWindowClosing()));
}

void GameManagerWindow::switchToStage1()
{

}

void GameManagerWindow::switchToStage2()
{

}

void GameManagerWindow::switchToStage3()
{

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

void GameManagerWindow::manageQmlWindowClosing()
{
    closureTimer->setSingleShot(true);
    closureTimer->start(QUIT_APP_ON_QML_CLOSURE_IN_N_MS);
    qDebug().noquote() << "QML Engine: Deleting because of closing window...";
    rootObj->deleteLater();
    qmlEngine->deleteLater();
    qDebug().noquote() << QString("QML Engine: Deleting done successfuly. Quiting app in %1 seconds...")
                          .arg(QString::number(QUIT_APP_ON_QML_CLOSURE_IN_N_MS/1000));
    connect(closureTimer, &QTimer::timeout, this, &QCoreApplication::quit);
}

void GameManagerWindow::applyFirstGameContext()
{
    fillDebugMapContext();
    //fillDebugPlayersContext();

    m_cellsList->setItems(debugMapContext);
    //m_playersList->setItems(debugPlayersContext);
}
