#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "helpers/basewin.h"
#include "helpers/common/filemanager.h"

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
    void show();
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

private:
    void closeEvent(QCloseEvent *event);
    void definePrivilege();
    void setUpByPrivilege();
    void setButtonsVisibility(bool areVisible);
    void setSettingsInputsAccessibility(bool areAccessible);
    void setUpSettingsInputs();
    void setUpUsersInTable(QTableWidget &table, vector<UserShortInfo>& usiContextVec);
    void tableClear(QTableWidget &table);
    QString countAverageRp();
    QString findOwnerNickname(int ownerId);
    LobbySettings makeSettingsObjectByInputs();
    void overwriteSettingsInputs(LobbySettings &overwriteBy);
    bool checkIfEveryoneReady();

private:
    Ui::LobbyWindow *ui;

    LobbyFullInfo m_context;
    LobbySettings m_lastSettings;

    enum PrivelegeTypes { Owner, Guest, RankedGuest };
    short m_privilegeType = PrivelegeTypes::Guest;
};

#endif // LOBBYWINDOW_H
