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
    void switchToStage1();
    void switchToStage2();
    void switchToStage3();
    void changePlayerBalance();
    void changePlayerNickname();
    void rollDiceDirectional();
    void rollDiceInfinite();
    void addPlayer();
    void removePlayer();
    void placePlayerPieceOn();
    void endTheGame();

private slots:
    void manageQmlWindowClosing();

private:
    void applyFirstGameContext(/*GameContext& gameContext*/);

private:
    Ui::GameManagerWindow *ui;
    QQmlApplicationEngine* qmlEngine;
    GameTransmitterObject* gameTransmitterObj;
    QObject* gameReceiverObj;

    CellsList* m_cellsList;
};

// DEBUG! Delete later
static Cell debugCellsArray[] =
{
    {8, Sawmill, NoPlayer, 0, 0, 0, 1}, {9, Common, NoPlayer, 0, 0, 0, 1}, {10, Sawmill, NoPlayer, 0, 0, 0, 1}, {11, CoalStation, NoPlayer, 0, 0, 0, 1}, {12, Common, NoPlayer, 0, 0, 0, 1}, {13, Sawmill, NoPlayer, 0, 0, 0, 1}, {14, Common, NoPlayer, 0, 0, 0, 1}, {15, Common, NoPlayer, 0, 0, 0, 1}, {16, Sawmill, NoPlayer, 0, 0, 0, 1},
    {7, Common, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {17, Common, NoPlayer, 0, 0, 0, 1},
    {6, Vacation, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {18, Common, NoPlayer, 0, 0, 0, 1},
    {5, Common, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {19, Vacation, NoPlayer, 0, 0, 0, 1},
    {4, Common, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {20, Common, NoPlayer, 0, 0, 0, 1},
    {3, CoalStation, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {21, CoalStation, NoPlayer, 0, 0, 0, 1},
    {2, Sawmill, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {22, Sawmill, NoPlayer, 0, 0, 0, 1},
    {1, Common, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {-1, Void, NoPlayer, 0, 0, 0, 1}, {23, Common, NoPlayer, 0, 0, 0, 1},
    {0, Beginning, NoPlayer, 0, 0, 0, 1}, {31, CoalStation, NoPlayer, 0, 0, 0, 1}, {30, Common, NoPlayer, 0, 0, 0, 1}, {29, CoalStation, NoPlayer, 0, 0, 0, 1}, {28, Common, NoPlayer, 0, 0, 0, 1}, {27, Sawmill, NoPlayer, 0, 0, 0, 1}, {26, Common, NoPlayer, 0, 0, 0, 1}, {25, Sawmill, NoPlayer, 0, 0, 0, 1}, {24, CoalStation, NoPlayer, 0, 0, 0, 1}
};

static CellsList debugMapContext;

static void fillDebugMapContext()
{
    for(int i = 0; i < 81; i++)
        debugMapContext.appendItem(debugCellsArray[i]);
}

#endif // GAMEMANAGERWINDOW_H
