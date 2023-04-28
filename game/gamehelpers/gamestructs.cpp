#include "gamestructs.h"

bool Cell::operator==(const Cell &cellContext)
{
    if((this->orderIndex == cellContext.orderIndex) &&
    (this->fieldTypeSet == cellContext.fieldTypeSet) &&
    (this->playerNumberOwner == cellContext.playerNumberOwner) &&
    (this->fieldCost == cellContext.fieldCost) &&
    (this->fieldIncome == cellContext.fieldIncome) &&
    (this->piecesOnCellMask == cellContext.piecesOnCellMask))
        return true;
    return false;
}

bool Cell::operator==(Cell &cellContext)
{
    if((this->orderIndex == cellContext.orderIndex) &&
    (this->fieldTypeSet == cellContext.fieldTypeSet) &&
    (this->playerNumberOwner == cellContext.playerNumberOwner) &&
    (this->fieldCost == cellContext.fieldCost) &&
    (this->fieldIncome == cellContext.fieldIncome) &&
    (this->piecesOnCellMask == cellContext.piecesOnCellMask))
        return true;
    return false;
}
