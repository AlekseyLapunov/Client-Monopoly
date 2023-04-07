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
    explicit LobbyWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                         unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                         QWidget *parent = nullptr);
    ~LobbyWindow();

    void giveFirstContext(LobbyFullInfo &context);
    void windowDataRefresh();

signals:
    void goToMenuWindow();

private slots:
    void toggleLobbyVision();
    void startGame();
    void setLobbySettings();
    void setGameSettings();
    void leaveLobby();
    void toggleReadyStatus();
    void quitApp();

private:
    void definePrivilege();
    void setUpByPrivilege();
    void setButtonsVisibility(bool areVisible);
    void setUpLobbySystem(LobbySystemInfo& lsiContext);
    void setUpGameSettings(GameSettingsInfo& gsContext);
    void setUpUsersInTable(vector<UserShortInfo>& usiContextVec);

private:
    Ui::LobbyWindow *ui;

    LobbyFullInfo m_context;

    enum privelegeTypes { owner, guest, rankedGuest };
    short m_privilegeType = guest;
};

#endif // LOBBYWINDOW_H
