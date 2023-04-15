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

    ui->setupUi(this);
}

GameManagerWindow::~GameManagerWindow()
{
    delete ui;
}

void GameManagerWindow::show()
{

    QWidget::show();
}
