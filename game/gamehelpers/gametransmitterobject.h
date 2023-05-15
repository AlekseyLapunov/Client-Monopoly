#ifndef GAMETRANSMITTEROBJECT_H
#define GAMETRANSMITTEROBJECT_H

#include <QObject>

#include "gamestructs.h"

class GameTransmitterObject : public QObject
{
    Q_OBJECT
public:
    explicit GameTransmitterObject(QObject *parent = nullptr);

    void emitFieldUpdate(int index, Cell newCell);

signals:
    // Global QML Update signals
    void setCurrentGameStage(uint8_t stageNumber, bool withAnimation = true);
    void manageActionMode(bool actionFlag);
    void finishTheGame();
    void arrowResolve(int orderIndex, int rotateCost);

    // Dice Animations signals
    void startDiceInfiniteAnimation();
    void startDiceDirectionalAnimation(uint8_t leftDiceValue, uint8_t rightDiceValue);
    void stopDiceAnimation();

    // Host Info Update signals
    void setHostPlayerNumber(uint8_t newHostPlayerNumber);
    void hostIncomeUpdate(int newHostIncome);
    void hostOwningObjectsUpdate(uint8_t owningObjectId, short owningObjectsCount);
    void hostOwningObjectsClear();

    // Players Model Update signals
    void playerNicknameUpdate(uint8_t playerNumber, QString newNickname);
    void playerBalanceUpdate(uint8_t playerNumber, int newBalance);
    void playerPiecePositionUpdate(uint8_t playerNumber, int piecePositionOnOrderIndex);
    void appendPlayer(uint8_t playerNumber, QString displayableName, int currentBalance,
                      int piecePositionOnOrderIndex);
    void deletePlayer(uint8_t playerNumber);

    // Fields Update signals
    void fieldUpdate(int index, int newOrderIndex, uint8_t newFieldTypeSet, uint8_t newPlayerNumberOwner,
                     int newFieldCost, int newFieldIncome, uint8_t newPiecesOnCellMask, uint8_t newStage,
                     uint8_t newArrowDirection, uint8_t newBlankUntilStage);
    void fieldOrderIndexUpdate(int index, int newOrderIndex);
    void fieldTypeUpdate(int index, uint8_t newFieldType);
    void fieldPlayerNumberOwnerUpdate(int index, uint8_t newPlayerNumberOwner);
    void fieldCostUpdate(int index, int newFieldCost);
    void fieldIncomeUpdate(int index, int newFieldIncome);
    void fieldPiecesMaskUpdate(int index, uint8_t newPiecesOnCellMask);
    void fieldStageUpdate(int index, uint8_t newStage);
    void fieldArrowDirectionUpdate(int index, uint8_t newDirection);
    void fieldBlankUntilStageUpdate(int index, uint8_t newBlankUntilStage);
};

#endif // GAMETRANSMITTEROBJECT_H
