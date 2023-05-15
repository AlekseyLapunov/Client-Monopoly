#ifndef GAMEMANAGERWINDOW_H
#define GAMEMANAGERWINDOW_H

#include <QWidget>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

#include "helpers/basewin.h"
#include "game/gamehelpers/gametransmitterobject.h"
#include "game/gamehelpers/gamealgo.h"
#include "game/models/fieldsgridmodel.h"
#include "game/models/playersinfomodel.h"
#include "game/models/lists/cellslist.h"
#include "game/models/lists/playergameinfolist.h"
#include "game/gamehelpers/gamestructs.h"

#define DEBUG_SHOW_DEBUG_PANEL

#define QUIT_APP_ON_QML_CLOSURE_IN_N_MS 1500

namespace Ui {
class GameManagerWindow;
}

class GameManagerWindow : public QWidget, BaseWin
{
    Q_OBJECT

public:
    explicit GameManagerWindow(unique_ptr<ServerCommunicator> *newServerPtr,
                               unique_ptr<UserMetaInfo> *newMetaInfoPtr,
                               QWidget *parent = nullptr);
    ~GameManagerWindow();

    void show();
    void closeEvent(QCloseEvent *event);

    void startQmlEngine();

signals:
    void goToMenuWindow();

private slots:
    void setStage();
    void changePlayerBalance();
    void changePlayerNickname();
    void rollDiceDirectional();
    void rollDiceInfinite();
    void addPlayer();
    void removePlayer();
    void placePlayerPieceOn();
    void endTheGame();
    void reactToPlayerNumberComboBoxChange();
    void allowAction(bool allowActionFlag);
    void showTraceAlgo();
    void makePlayerHost();
    void showArrowResolve();

private slots:
    void manageQmlWindowClosing();
    void manageBuyClicked();
    void manageSabotageResult(int chosenOrderIndex);

private:
    void applyFirstGameContext(/*GameContext& gameContext*/);
    void checkTimedOutCounter();
    void initializeHostOwningObjectCounts();
    void countHostOwningObjectsByMap();
    void resetHostOwningObjectCounts();
    void passOwningObjectsToQml();
    bool checkIfOrderIndexIsValid(int inputOrderIndex);
    void changePiecesMaskByOrderIndex(int inputOrderIndex,
                                      uint8_t whatPlayerNumber,
                                      uint8_t addOrRemove);
    vector<int> getValuesFromPteAsRealIndexes();

private:
    enum ChangingPiecesMask { DeletePM, AddPM };

    Ui::GameManagerWindow *ui;
    QQmlApplicationEngine* qmlEngine;
    GameTransmitterObject* gameTransmitterObj;
    QObject* gameReceiverObj;

    uint8_t currentStage = 0;
    int hostPlayerIncome = 0;
    uint8_t hostPlayerNumber = PlayerNumber::Player2; // STUB
    QMap<uint8_t, short> hostOwningObjectsCounts;

    CellsList* m_cellsList;
    PlayerGameInfoList* m_playersList;
};


// DEBUG!
static Cell debugCellsArray[] =
{
    {9, Sawmill, Player2, 50, 5, 0, 1, 0, 1}, {10, Common, NoPlayer, 0, 0, 0, 1, 0, 1}, {11, Sawmill, NoPlayer, 50, 5, 0, 1, 0, 1}, {12, CoalStation, Player1, 150, 20, 0, 1, 0, 1}, {13, Arrow, NoPlayer, 0, 0, 0, 1, ArrowRight, 2}, {14, Sawmill, Player2, 50, 5, 0, 1, 0, 1}, {15, Forest, NoPlayer, 100, 0, 0, 1, 0, 2}, {16, Sabotage, NoPlayer, 0, 0, 0, 1, 0, 3}, {17, Sawmill, Player3, 50, 5, 0, 1, 0, 1},
    {8, Forest, NoPlayer, 100, 0, 0, 1, 0, 2}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {33, Uranium, NoPlayer, 500, 0, 0, 2, 0, 3}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {18, Uranium, NoPlayer, 500, 0, 0, 1, 0, 3},
    {7, Vacation, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {34, Coal, NoPlayer, 250, 0, 0, 2, 0, 2}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {19, Common, NoPlayer, 0, 0, 0, 1, 0, 1},
    {6, Common, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {35, Common, NoPlayer, 0, 0, 0, 2, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {20, Vacation, NoPlayer, 0, 0, 0, 1, 0, 1},
    {5, Arrow, NoPlayer, 0, 0, 0, 1, ArrowUp, 3}, {45, Sabotage, NoPlayer, 0, 0, 0, 3, 0, 3}, {44, Coal, NoPlayer, 250, 0, 0, 3, 0, 2}, {43, Common, NoPlayer, 0, 0, 0, 3, 0, 3}, {36, Arrow, NoPlayer, 0, 0, 0, 2, ArrowLeft, 3}, {42, Sabotage, NoPlayer, 0, 0, 0, 3, 0, 3}, {41, Coal, NoPlayer, 250, 0, 0, 3, 0, 2}, {40, Common, NoPlayer, 0, 0, 0, 3, 0, 3}, {21, Arrow, NoPlayer, 0, 0, 0, 1, ArrowDown, 3},
    {4, CoalStation, NoPlayer, 150, 20, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {37, Common, NoPlayer, 0, 0, 0, 2, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {22, AtomicStation, NoPlayer, 350, 50, 0, 1, 0, 3},
    {3, Sawmill, NoPlayer, 50, 5, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {38, AtomicStation, NoPlayer, 350, 50, 0, 2, 0, 3}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {23, Sawmill, NoPlayer, 50, 5, 0, 1, 0, 1},
    {2, Uranium, Player3, 500, 0, 0, 1, 0, 3}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {39, Common, NoPlayer, 0, 0, 0, 2, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {24, Common, NoPlayer, 0, 0, 0, 1, 0, 1},
    {1, Beginning, NoPlayer, 0, 0, 0b000011, 1, 0, 1}, {32, AtomicStation, Player3, 350, 50, 0, 1, 0, 3}, {31, Common, NoPlayer, 0, 0, 0, 1, 0, 1}, {30, CoalStation, Player2, 150, 20, 0, 1, 0, 1}, {29, Arrow, NoPlayer, 0, 0, 0, 1, ArrowLeft, 2}, {28, Sawmill, NoPlayer, 50, 5, 0, 1, 0, 1}, {27, Forest, NoPlayer, 100, 0, 0, 1, 0, 2}, {26, Sawmill, Player2, 50, 5, 0, 1, 0, 1}, {25, CoalStation, Player5, 150, 20, 0, 1, 0, 1}
};

static CellsList debugMapContext;

static void fillDebugMapContext();

static PlayerGameInfo debugPlayersArray[] =
{
    { PlayerNumber::Player1, "Player1", 200, 1 },
    { PlayerNumber::Player2, "Player2", 210, 1  }
};

static PlayerGameInfoList debugPlayersContext;

static void fillDebugPlayersContext();

#endif // GAMEMANAGERWINDOW_H
