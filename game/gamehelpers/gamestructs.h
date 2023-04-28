#ifndef GAMESTRUCTS_H
#define GAMESTRUCTS_H

#include <memory>
#include <vector>

using std::vector;

typedef unsigned short ushort;

#define DEFAULT_MAP_DIMENSION 9

struct Cell
{
    bool operator==(const Cell &cellContext);
    bool operator==(Cell &cellContext);

    int orderIndex;
    ushort fieldTypeSet;
    ushort playerNumberOwner;
    int fieldCost;
    int fieldIncome;
    ushort piecesOnCellMask;
};

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

#endif // GAMESTRUCTS_H
