#ifndef GAMESTRUCTS_H
#define GAMESTRUCTS_H

#include <memory>
#include <stdint.h>
#include <vector>

using std::vector;

#define DEFAULT_MAP_DIMENSION 9

struct Cell
{
    bool operator==(const Cell &cellContext);
    bool operator==(Cell &cellContext);

    int orderIndex;
    uint8_t fieldTypeSet;
    uint8_t playerNumberOwner;
    int fieldCost;
    int fieldIncome;
    uint8_t piecesOnCellMask;
    uint8_t stage;

    uint8_t arrowDirection;
    uint8_t blankUntilStage;
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

enum ArrowRotation
{
    North, East, South, West
};

#endif // GAMESTRUCTS_H
