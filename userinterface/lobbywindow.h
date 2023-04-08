#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QTableWidget>

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
    void showAndRefresh();

signals:
    void goToMenuWindow();

private slots:
    void toggleLobbyVision();
    void startGame();
    void applySettings();
    void leaveLobby();
    void toggleReadyStatus();
    void quitApp();
    void settingsChangesDetected();
    void toggleMaxBalanceAccessibility();
    void toggleMaxTurnsAccessibility();
    void restoreLastSettings();

private:
    void definePrivilege();
    void setUpByPrivilege();
    void setButtonsVisibility(bool areVisible);
    void setSettingsInputsAccessibility(bool areAccessible);
    void setUpLobbySystem(LobbySystemInfo& lsiContext);
    void setUpGameSettings(GameSettingsInfo& gsContext);
    void setUpUsersInTable(QTableWidget &table, vector<UserShortInfo>& usiContextVec);
    void tableClear(QTableWidget &table);
    QString countAverageRp();
    QString findOwnerNickname(int ownerId);
    LobbySettingsCombined makeSettingsObject();
    void overwriteSettingsInputs(LobbySettingsCombined &overwriteBy);
    bool checkIfEveryoneReady();

private:
    Ui::LobbyWindow *ui;

    LobbyFullInfo m_context;
    LobbySettingsCombined m_lastSettings;

    enum PrivelegeTypes { Owner, Guest, RankedGuest };
    short m_privilegeType = PrivelegeTypes::Guest;
};

#endif // LOBBYWINDOW_H
