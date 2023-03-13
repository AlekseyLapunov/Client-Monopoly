#include "lobbywindow.h"
#include "ui_lobbywindow.h"

LobbyWindow::LobbyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LobbyWindow)
{
    ui->setupUi(this);
}

LobbyWindow::~LobbyWindow()
{
    delete ui;
}

void LobbyWindow::toggleLobbyVision()
{

}

void LobbyWindow::startGame()
{

}

void LobbyWindow::setLobbySettings()
{

}

void LobbyWindow::setGameSettings()
{

}

void LobbyWindow::leaveLobby()
{

}

void LobbyWindow::toggleReadyStatus()
{

}

void LobbyWindow::quitApp()
{
    QCoreApplication::quit();
}
