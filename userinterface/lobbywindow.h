#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>

#include "helpers/basewin.h"

namespace Ui {
class LobbyWindow;
}

class LobbyWindow : public QMainWindow, public BaseWin
{
    Q_OBJECT

public:
    explicit LobbyWindow(QWidget *parent = nullptr);
    ~LobbyWindow();

private slots:
    void toggleLobbyVision();
    void startGame();
    void setLobbySettings();
    void setGameSettings();
    void leaveLobby();
    void toggleReadyStatus();
    void quitApp();


private:
    Ui::LobbyWindow *ui;
};

#endif // LOBBYWINDOW_H
