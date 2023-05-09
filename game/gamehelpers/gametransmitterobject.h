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
    void fieldPiecesMaskChanged(int index, uint8_t piecesOnCellMask);
    void fieldChanged(int index, int orderIndex, uint8_t fieldTypeSet, uint8_t playerNumberOwner,
                      int fieldCost, int fieldIncome, uint8_t piecesOnCellMask, uint8_t stage,
                      uint8_t arrowDirection, uint8_t blankUntilStage);
    void startDiceInfiniteAnimation();
    void startDiceDirectionalAnimation(uint8_t leftDiceValue, uint8_t rightDiceValue);
    void stopDiceAnimation();
    void appendPlayer(uint8_t playerNumber, QString displayableName, uint8_t currentBalance);
    void deletePlayer(uint8_t playerNumber);
    void updateHostOwningObjects(uint8_t owningObjectId, short owningObjectsCount);
    void clearHostOwningObjects();
    void updateHostIncomeInfo(int hostIncome);
    void updatePlayerNickname(uint8_t playerNumber, QString newNickname);
    void updatePlayerBalance(uint8_t playerNumber, int newBalance);
    void setHostPlayerNumber(uint8_t hostPlayerNumberToSet);
};

#endif // GAMETRANSMITTEROBJECT_H
