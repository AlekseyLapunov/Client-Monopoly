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