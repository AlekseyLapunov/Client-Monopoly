#include "gametransmitterobject.h"

GameTransmitterObject::GameTransmitterObject(QObject *parent)
    : QObject{parent}
{

}

void GameTransmitterObject::emitFieldUpdate(int index, Cell newCell)
{
    emit fieldUpdate(index, newCell.orderIndex, newCell.fieldTypeSet, newCell.playerNumberOwner,
                      newCell.fieldCost, newCell.fieldIncome, newCell.piecesOnCellMask,
                      newCell.stage, newCell.arrowDirection,  newCell.blankUntilStage);
}
