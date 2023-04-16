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
    if(qmlEngine != nullptr)
    {
        qDebug() << "Already created";
        return;
    }
    qmlEngine = new QQmlApplicationEngine;
    qmlEngine->load("qrc:/qmlfiles/GameWindow.qml");
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
