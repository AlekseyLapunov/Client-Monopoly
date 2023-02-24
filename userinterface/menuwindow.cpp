#include "menuwindow.h"
#include "ui_menuwindow.h"

MenuWindow::MenuWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuWindow)
{
    ui->setupUi(this);

    QRegularExpression lobbyFilterRegExp("");
    QRegularExpressionValidator *lobbyFilterValidator = new QRegularExpressionValidator(lobbyFilterRegExp, this);
    ui->leLobbyFilter->setValidator(lobbyFilterValidator);
}

MenuWindow::~MenuWindow()
{
    delete ui;
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
