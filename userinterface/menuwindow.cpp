#include "menuwindow.h"
#include "ui_menuwindow.h"

MenuWindow::MenuWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                       unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuWindow)
{
    setupPointers(*newServerPtr, *newMetaInfoPtr);

    ui->setupUi(this);
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::windowDataRefresh()
{
    QRegularExpression lobbyFilterRegExp("/^[a-zA-Z0-9]+ ?[a-zA-Z0-9]*$/");
    QRegularExpressionValidator *lobbyFilterValidator = new QRegularExpressionValidator(lobbyFilterRegExp, this);
    ui->leLobbyFilter->setValidator(lobbyFilterValidator);

    ui->lNickname->setText(pUserMetaInfo()->get()->getHostInfo().userName);
    ui->lRpCount->setText(QString::number(pUserMetaInfo()->get()->getHostInfo().userRpCount));
}

void MenuWindow::quitApp()
{
    QMessageBox* quitingAppDialog = makeDialogBox(QMessageBox::Question,
                                                  "Выход из приложения",
                                                  "Вы уверены, что хотите выйти из приложения?",
                                                  {"Да", "Нет"});
    int answer = quitingAppDialog->exec();
    delete quitingAppDialog;
    if(answer == 0)
        QCoreApplication::quit();
}

void MenuWindow::changeAcc()
{
    this->hide();
    emit goToLoginWindow();
}
