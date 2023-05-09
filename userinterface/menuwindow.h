#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>

#include "subinterfaces/menusubdialog.h"
#include "subinterfaces/gamemanagerwindow.h"
#include "helpers/basewin.h"
#include "lobbywindow.h"

#define DEBUG_FAST_LOAD_INTO_THE_GAME

typedef short dialogCode;

namespace Ui {
class MenuWindow;
}

class MenuWindow : public QMainWindow, public BaseWin
{
    Q_OBJECT

public:
    explicit MenuWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                        unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                        QWidget *parent = nullptr);
    ~MenuWindow();

    void windowDataRefresh();
    void quitAppDialog();
    void show();
    void hide();
    void supportLogoutChain();

signals:
    void switchToLoginWindow();

private slots:
    void quitApp();
    void changeAcc();
    void apply3dDiceState();
    void lobbyClicked(QTableWidgetItem *itemClicked);
    void applyLobbyFilter(QString textChanged);
    void joinToLobby(QTableWidgetItem *itemActivated);
    void joinToLobby();
    void joinIdDialog();
    void createLobby();
    void findRanked();
    void showAbout();
    void chooseNickname();
    void checkStatusBarToClear();

private:
    void closeEvent(QCloseEvent *event);
    void setupLobbiesTable();
    void setupLobbiesFilter();
    void displayHostShortInfo();
    void tableClear(QTableWidget &table);
    void tableSetupFill(QTableWidget &table, const vector<LobbyShortInfo> &contentVec, const QString &filter = "");
    void switchJoinByItem(const QTableWidgetItem &item);
    void showLobbyWindow();
    void logoutBackToLoginWindow();
    void checkTimedOutCounter();

    enum DialogCodes { NoPassword, PassRejected, PassEntered };
    dialogCode checkIfPassworded(const QTableWidgetItem &item);

private:
    Ui::MenuWindow *ui;
    unique_ptr<MenuSubDialog> pSubDialog;
    unique_ptr<LobbyWindow> pLobbyWindow;
    unique_ptr<GameManagerWindow> pGameManagerWindow;

    LobbyFullInfo m_firstContext;

    int selectedLobbyUniqueId = -1;
};

#endif // MENUWINDOW_H
