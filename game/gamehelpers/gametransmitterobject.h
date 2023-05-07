#ifndef GAMETRANSMITTEROBJECT_H
#define GAMETRANSMITTEROBJECT_H

#include <QObject>

#include "gamestructs.h"

class GameTransmitterObject : public QObject
{
    Q_OBJECT
public:
    explicit GameTransmitterObject(QObject *parent = nullptr);

    void emitFieldChanged(int index, Cell newCell);

signals:
    void setCurrentGameStage(uint8_t stageNumber, bool withAnimation = true);
    void fieldOrderIndexChanged(int index, int orderIndex);
    void fieldChanged(int index, int orderIndex, uint8_t fieldTypeSet, uint8_t playerNumberOwner,
                      int fieldCost, int fieldIncome, uint8_t piecesOnCellMask, uint8_t stage,
                      uint8_t arrowDirection, uint8_t blankUntilStage);
};

#endif // GAMETRANSMITTEROBJECT_H
