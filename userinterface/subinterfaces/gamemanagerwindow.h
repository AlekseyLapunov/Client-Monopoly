#ifndef GAMEMANAGERWINDOW_H
#define GAMEMANAGERWINDOW_H

#include <QWidget>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

#include "helpers/basewin.h"
#include "game/gamehelpers/gametransmitterobject.h"
#include "game/models/fieldsgridmodel.h"
#include "game/models/lists/cellslist.h"
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
    void fakePlayerTurn();
    void reactToPlayerNumberSpinBoxChange();

private slots:
    void manageQmlWindowClosing();

private:
    void applyFirstGameContext(/*GameContext& gameContext*/);

private:
    Ui::GameManagerWindow *ui;
    QQmlApplicationEngine* qmlEngine;
    GameTransmitterObject* gameTransmitterObj;
    QObject* gameReceiverObj;

    uint8_t currentStage = 0;

    CellsList* m_cellsList;
};

// DEBUG!
static Cell debugCellsArray[] =
{
    {8, Sawmill, Player2, 50, 5, 0, 1, 0, 1}, {9, Common, NoPlayer, 0, 0, 0, 1, 0, 1}, {10, Sawmill, NoPlayer, 50, 5, 0, 1, 0, 1}, {11, CoalStation, Player1, 150, 20, 0, 1, 0, 1}, {12, Arrow, NoPlayer, 0, 0, 0, 1, East, 2}, {13, Sawmill, NoPlayer, 50, 5, 0, 1, 0, 1}, {14, Forest, NoPlayer, 100, 0, 0, 1, 0, 2}, {15, Sabotage, NoPlayer, 0, 0, 0, 1, 0, 3}, {16, Sawmill, Player3, 50, 5, 0, 1, 0, 1},
    {7, Forest, NoPlayer, 100, 0, 0, 1, 0, 2}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Uranium, NoPlayer, 500, 0, 0, 2, 0, 3}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {17, Uranium, NoPlayer, 500, 0, 0, 1, 0, 3},
    {6, Vacation, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Coal, NoPlayer, 250, 0, 0, 2, 0, 2}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {18, Common, NoPlayer, 0, 0, 0, 1, 0, 1},
    {5, Common, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Common, NoPlayer, 0, 0, 0, 2, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {19, Vacation, NoPlayer, 0, 0, 0, 1, 0, 1},
    {4, Arrow, NoPlayer, 0, 0, 0, 1, North, 3}, {-1, Sabotage, NoPlayer, 0, 0, 0, 3, 0, 3}, {-1, Coal, NoPlayer, 250, 0, 0, 3, 0, 2}, {-1, Common, NoPlayer, 0, 0, 0, 3, 0, 3}, {-1, Arrow, NoPlayer, 0, 0, 0, 2, 0, 3}, {-1, Sabotage, NoPlayer, 0, 0, 0, 3, 0, 3}, {-1, Coal, NoPlayer, 250, 0, 0, 3, 0, 2}, {-1, Common, NoPlayer, 0, 0, 0, 3, 0, 3}, {20, Arrow, NoPlayer, 0, 0, 0, 1, South, 3},
    {3, CoalStation, NoPlayer, 150, 20, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Common, NoPlayer, 0, 0, 0, 2, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {21, AtomicStation, NoPlayer, 350, 50, 0, 1, 0, 3},
    {2, Sawmill, NoPlayer, 50, 5, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, AtomicStation, NoPlayer, 350, 50, 0, 2, 0, 3}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {22, Sawmill, NoPlayer, 50, 5, 0, 1, 0, 1},
    {1, Uranium, NoPlayer, 500, 0, 0, 1, 0, 3}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Common, NoPlayer, 0, 0, 0, 2, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1, 0, 1}, {23, Common, NoPlayer, 0, 0, 0, 1, 0, 1},
    {0, Beginning, NoPlayer, 0, 0, 0b000011, 1, 0, 1}, {31, AtomicStation, NoPlayer, 350, 50, 0, 1, 0, 3}, {30, Common, NoPlayer, 0, 0, 0, 1, 0, 1}, {29, CoalStation, NoPlayer, 150, 20, 0, 1, 0, 1}, {28, Arrow, NoPlayer, 0, 0, 0, 1, West, 2}, {27, Sawmill, NoPlayer, 50, 5, 0, 1, 0, 1}, {26, Forest, NoPlayer, 100, 0, 0, 1, 0, 2}, {25, Sawmill, NoPlayer, 50, 5, 0, 1, 0, 1}, {24, CoalStation, Player5, 150, 20, 0, 1, 0, 1}
};

static CellsList debugMapContext;

static void fillDebugMapContext()
{
    for(int i = 0; i < 81; i++)
        debugMapContext.appendItem(debugCellsArray[i]);
}

#endif // GAMEMANAGERWINDOW_H
