#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "helpers/basewin.h"
#include "helpers/common/filemanager.h"
#include "subinterfaces/gamemanagerwindow.h"

//#define DEBUG

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

private:
    Ui::LobbyWindow *ui;

    LobbyFullInfo m_context;
    LobbySettings m_lastSettings;

    unique_ptr<GameManagerWindow> *m_pGameManager;

    enum PrivelegeTypes { Owner, Guest, RankedGuest };
};

#endif // LOBBYWINDOW_H
