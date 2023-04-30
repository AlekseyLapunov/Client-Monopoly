#ifndef GAMEMANAGERWINDOW_H
#define GAMEMANAGERWINDOW_H

#include <QWidget>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

#include "helpers/basewin.h"
#include "game/models/fieldsgridmodel.h"
#include "game/models/lists/cellslist.h"
#include "game/gamehelpers/gamestructs.h"

//#define DEBUG

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
    void quitAppDialog();
    void closeEvent(QCloseEvent *event);

    void startQmlEngine();

private:
    void applyFirstGameContext(/*GameContext& gameContext*/);

private:
    Ui::GameManagerWindow *ui;
    QQmlApplicationEngine* qmlEngine = nullptr;

    CellsList* m_cellsList;
};

// DEBUG! Delete later
static Cell debugCellsArray[] =
{
    {8, Sawmill, NoPlayer, 0, 0, 0}, {9, Common, NoPlayer, 0, 0, 0}, {10, Sawmill, NoPlayer, 0, 0, 0}, {11, CoalStation, NoPlayer, 0, 0, 0}, {12, Common, NoPlayer, 0, 0, 0}, {13, Sawmill, NoPlayer, 0, 0, 0}, {14, Common, NoPlayer, 0, 0, 0}, {15, Common, NoPlayer, 0, 0, 0}, {16, Sawmill, NoPlayer, 0, 0, 0},
    {7, Common, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {17, Common, NoPlayer, 0, 0, 0},
    {6, Vacation, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {18, Common, NoPlayer, 0, 0, 0},
    {5, Common, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {19, Vacation, NoPlayer, 0, 0, 0},
    {4, Common, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {20, Common, NoPlayer, 0, 0, 0},
    {3, CoalStation, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {21, CoalStation, NoPlayer, 0, 0, 0},
    {2, Sawmill, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {22, Sawmill, NoPlayer, 0, 0, 0},
    {1, Common, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {-1, Void, NoPlayer, 0, 0, 0}, {23, Common, NoPlayer, 0, 0, 0},
    {0, Beginning, NoPlayer, 0, 0, 0}, {31, CoalStation, NoPlayer, 0, 0, 0}, {30, Common, NoPlayer, 0, 0, 0}, {29, CoalStation, NoPlayer, 0, 0, 0}, {28, Common, NoPlayer, 0, 0, 0}, {27, Sawmill, NoPlayer, 0, 0, 0}, {26, Common, NoPlayer, 0, 0, 0}, {25, Sawmill, NoPlayer, 0, 0, 0}, {24, CoalStation, NoPlayer, 0, 0, 0},
};

static CellsList debugMapContext;

static void fillDebugMapContext()
{
    for(int i = 0; i < 81; i++)
        debugMapContext.appendItem(debugCellsArray[i]);
}

#endif // GAMEMANAGERWINDOW_H
