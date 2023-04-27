#ifndef GAMESTRUCTS_H
#define GAMESTRUCTS_H

#include <memory>
#include <vector>
using std::vector;

typedef unsigned short ushort;

#define DEFAULT_MAP_DIMENSION 9

enum FieldType
{
    Void, Common, Beginning, Sawmill, CoalStation,
    AtomicStation, Forest, Coal, Uranium, Vacation,
    Sabotage, Arrow
};

enum PlayerNumber
{
    NoPlayer, Player1, Player2, Player3,
    Player4, Player5, Player6
};

enum Dice
{
    Left, Right
};

struct Cell
{
    Cell& operator=(const Cell &cellContext);
    bool operator==(const Cell &cellContext);

    int orderIndex;
    ushort fieldType;
    ushort playerNumberOwner;
    int fieldCost;
    int fieldIncome;
    ushort piecesOnCellMask;
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

static vector<Cell> debugMapContext;

static void fillDebugMapContext()
{
    for(int i = 0; i < 81; i++)
        debugMapContext.push_back(debugCellsArray[i]);
}

#endif // GAMESTRUCTS_H
