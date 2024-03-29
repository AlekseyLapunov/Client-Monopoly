#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "helpers/basewin.h"
#include "helpers/common/filemanager.h"
#include "subinterfaces/menusubdialog.h"
#include "subinterfaces/gamemanagerwindow.h"

//#define DEBUG_FAST_GAME_START

#define TIME_TO_START_GAME_MS 5000

namespace Ui {
class LobbyWindow;
}

class LobbyWindow : public QMainWindow, public BaseWin
{
    Q_OBJECT

public:
    explicit LobbyWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                         unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                         unique_ptr<GameManagerWindow> &newGameManagerPtr,
                         QWidget *parent = nullptr);
    ~LobbyWindow();

    void windowDataRefresh();
    void show(const LobbyFullInfo firstContext = {DefaultLobbySettings, {}});
    void hide();
    void quitAppDialog();

signals:
    void goToMenuWindow();
    void initLogoutChain();

private slots:
    void startGame();
    void applySettings();
    void leaveLobby();
    void toggleReadyStatus();
    void quitApp();
    void settingsChangesDetected();
    void toggleMaxBalanceAccessibility();
    void toggleMaxTurnsAccessibility();
    void restoreLastSettings();
    void applyRankedSettings();
    void exportSettingsToFile();
    void importSettingsFromFile();
    void reactToUserSelect(QTableWidgetItem* item);
    void togglePasswordLineEditEcho();
    void deleteLobby();
    void changeNickname();
    void apply3dDiceState();
    void openGameWindow();

private:
    void setFirstContext(const LobbyFullInfo context);
    void closeEvent(QCloseEvent *event);
    short definePrivilege();
    void setUpByPrivilege();
    void setButtonsVisibility(bool areVisible);
    void setSettingsInputsAccessibility(bool areAccessible);
    void setUpSettingsInputs();
    void setUpRegExps();
    void setUpUsersInTable(QTableWidget &table, vector<UserShortInfo>& usiContextVec);
    void tableClear(QTableWidget &table);
    QString countAverageRp();
    QString findOwnerNickname(int ownerId);
    LobbySettings makeSettingsObjectByInputs();
    void overwriteSettingsInputs(LobbySettings &overwriteBy);
    bool checkIfEveryoneReady();
    void checkMaxPlayers();
    void switchBackToMenuWindow();
    void checkLimitationChecks();
    void logoutBackToLoginWindow();
    void checkTimedOutCounter();
    void manageCheckBoxes();
    void checkIfNeedToStartGame();

private:
    Ui::LobbyWindow *ui;

    LobbyFullInfo m_context;
    LobbySettings m_lastSettings;

    unique_ptr<GameManagerWindow> *m_pGameManager;

    QTimer timerToStartGame;
    QTimer subTimerToStartGame;
    bool timerAlreadyActive = false;

    enum PrivelegeTypes { Owner, JoinedUser, RankedJoinedUser };
};

#endif // LOBBYWINDOW_H
