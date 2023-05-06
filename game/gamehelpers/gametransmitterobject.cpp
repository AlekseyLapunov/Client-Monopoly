#include "gametransmitterobject.h"

GameTransmitterObject::GameTransmitterObject(QObject *parent)
    : QObject{parent}
{

}

void GameTransmitterObject::emitFieldChanged(int index, Cell newCell)
{
    emit fieldChanged(index, newCell.orderIndex, newCell.fieldTypeSet, newCell.playerNumberOwner,
                      newCell.fieldCost, newCell.fieldIncome, newCell.piecesOnCellMask,
                      newCell.stage, newCell.arrowDirection,  newCell.blankUntilStage);
}
