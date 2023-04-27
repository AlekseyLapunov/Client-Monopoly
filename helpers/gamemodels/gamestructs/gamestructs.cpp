#include "gamestructs.h"

Cell& Cell::operator=(const Cell &cellContext)
{
    this->orderIndex = cellContext.orderIndex;
    this->fieldType = cellContext.fieldType;
    this->playerNumberOwner = cellContext.playerNumberOwner;
    this->fieldCost = cellContext.fieldCost;
    this->fieldIncome = cellContext.fieldIncome;
    this->piecesOnCellMask = cellContext.piecesOnCellMask;
    return *this;
}

bool Cell::operator==(const Cell &cellContext)
{
    if((this->orderIndex == cellContext.orderIndex) &&
    (this->fieldType == cellContext.fieldType) &&
    (this->playerNumberOwner == cellContext.playerNumberOwner) &&
    (this->fieldCost == cellContext.fieldCost) &&
    (this->fieldIncome == cellContext.fieldIncome) &&
    (this->piecesOnCellMask == cellContext.piecesOnCellMask))
        return true;
    return false;
}
