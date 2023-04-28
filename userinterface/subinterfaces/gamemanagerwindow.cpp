#include "gamemanagerwindow.h"
#include "ui_gamemanagerwindow.h"

GameManagerWindow::GameManagerWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                                     unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                                     QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameManagerWindow)
{
    if(!*newServerPtr || !*newMetaInfoPtr)
        throw std::runtime_error(ssClassNames[GameManagerCN] + ssRuntimeErrors[PtrLinkFail]);

    setupPointers(*newServerPtr, *newMetaInfoPtr);
}

GameManagerWindow::~GameManagerWindow()
{
    delete qmlEngine;
    qmlEngine = nullptr;
    delete ui;
}

void GameManagerWindow::show()
{
    ui->setupUi(this);

    startQmlEngine();

#ifdef DEBUG
    QWidget::hide();
    return;
#endif
    QWidget::show();
}

void GameManagerWindow::quitAppDialog()
{
    if(makeDialog(BaseWin::QuitApp, "", this) == 0)
        QCoreApplication::quit();
}

void GameManagerWindow::closeEvent(QCloseEvent *event)
{
    quitAppDialog();
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
}

void GameManagerWindow::applyFirstGameContext()
{
    fillDebugMapContext();

    m_cellsList->setItems(debugMapContext);
}
