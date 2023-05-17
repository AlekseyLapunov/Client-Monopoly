#ifndef GAMETRANSMITTEROBJECT_H
#define GAMETRANSMITTEROBJECT_H

#include <QObject>

#include "gamestructs.h"

/*!
 * \brief GameTransmitterObject Класс взаимодействия с приложением QML. Наследник QObject.
 */
class GameTransmitterObject : public QObject
{
    Q_OBJECT
public:
    explicit GameTransmitterObject(QObject *parent = nullptr);

    /*!
     * \brief emitFieldUpdate Метод вызывает сигнал обновления игрового поля.
     * \param index Настоящий индекс поля на игровой сетке.
     * \param newCell Клетка с новыми значениями.
     */
    void emitFieldUpdate(int index, Cell newCell);

signals:

    // Global QML Update signals
    /*!
     * \brief setCurrentGameStage Сигнал установки номера текущей стадии игры.
     * \param stageNumber Номер стадии.
     * \param withAnimation Флаг "показывать анимацию или нет".
     */
    void setCurrentGameStage(uint8_t stageNumber, bool withAnimation = true);
    /*!
     * \brief manageActionMode Сигнал разрешения выполнять действие над активируемым полем.
     * \param actionFlag Флаг, равный true, если разрешить, и false - если нет.
     */
    void manageActionMode(bool actionFlag);
    /*!
     * \brief finishTheGame Сигнал окончания игры.
     */
    void finishTheGame();
    /*!
     * \brief arrowResolve Сигнал о том, что игроку нужно выбрать поворот стрелки (или отказаться от него).
     * \param orderIndex Порядковый номер поля-стрелки.
     * \param rotateCost Цена поворота "стрелки".
     */
    void arrowResolve(int orderIndex, int rotateCost);

    // Dice Animations signals
    /*!
     * \brief startDiceInfiniteAnimation Сигнал старта анимации "бесконечного" поворота игральных кубиков.
     */
    void startDiceInfiniteAnimation();
    /*!
     * \brief startDiceDirectionalAnimation Сигнал поворота игральных кубиков для отображения конкретных значений.
     * \param leftDiceValue Значение на левой игральной кости.
     * \param rightDiceValue Значение на правой игральной кости.
     */
    void startDiceDirectionalAnimation(uint8_t leftDiceValue, uint8_t rightDiceValue);
    /*!
     * \brief stopDiceAnimation Сигнал остановки анимации поворота игральных кубиков.
     */
    void stopDiceAnimation();

    // Host Info Update signals
    /*!
     * \brief setHostPlayerNumber Сигнал установки номера игрока для пользователя.
     * \param newHostPlayerNumber Номер игрока из перечисления.
     */
    void setHostPlayerNumber(uint8_t newHostPlayerNumber);
    /*!
     * \brief hostIncomeUpdate Сигнал обновления информации о доходе пользователя.
     * \param newHostIncome Доход в млн. ЭК.
     */
    void hostIncomeUpdate(int newHostIncome);
    /*!
     * \brief hostOwningObjectsUpdate Сигнал обновления данных пользователя о его владениях.
     * \param owningObjectId Идентификатор владения из перечисления типов полей.
     * \param owningObjectsCount Количество объектов владения.
     */
    void hostOwningObjectsUpdate(uint8_t owningObjectId, short owningObjectsCount);
    /*!
     * \brief hostOwningObjectsClear Сигнал очищения информации о владениях пользователя.
     */
    void hostOwningObjectsClear();

    // Players Model Update signals
    /*!
     * \brief playerNicknameUpdate Сигнал обновления псевдонима игрока.
     * \param playerNumber Номер игрока из перечисления.
     * \param newNickname Новый псевдоним.
     */
    void playerNicknameUpdate(uint8_t playerNumber, QString newNickname);
    /*!
     * \brief playerBalanceUpdate Сигнал обновления баланса игрока.
     * \param playerNumber Номер игрока из перечисления.
     * \param newBalance Новый баланс в млн. ЭК.
     */
    void playerBalanceUpdate(uint8_t playerNumber, int newBalance);
    /*!
     * \brief playerPiecePositionUpdate Сигнал обновления позиции фишки игрока.
     * \param playerNumber Номер игрока из перечисления.
     * \param piecePositionOnOrderIndex Порядковый номер клетки для расположения фишки.
     */
    void playerPiecePositionUpdate(uint8_t playerNumber, int piecePositionOnOrderIndex);
    /*!
     * \brief appendPlayer Сигнал добавления игрока. Параметры: значения полей структуры PlayerGameInfo.
     */
    void appendPlayer(uint8_t playerNumber, QString displayableName, int currentBalance,
                      int piecePositionOnOrderIndex);
    /*!
     * \brief deletePlayer Сигнал удаления игрока.
     * \param playerNumber Номер игрока из перечисления.
     */
    void deletePlayer(uint8_t playerNumber);

    // Fields Update signals
    /*!
     * \brief fieldUpdate Сигнал обновления поля. Параметры: значения полей структуры Cell.
     */
    void fieldUpdate(int index, int newOrderIndex, uint8_t newFieldTypeSet, uint8_t newPlayerNumberOwner,
                     int newFieldCost, int newFieldIncome, uint8_t newPiecesOnCellMask, uint8_t newStage,
                     uint8_t newArrowDirection, uint8_t newBlankUntilStage);
    /*!
     * \brief fieldOrderIndexUpdate Сигнал изменения порядкового номера поля.
     * \param index Настоящий индекс поля на сетке.
     * \param newOrderIndex Новый порядковый индекс.
     */
    void fieldOrderIndexUpdate(int index, int newOrderIndex);
    /*!
     * \brief fieldTypeUpdate Сигнал обновления типа поля.
     * \param index Настоящий индекс поля на сетке.
     * \param newFieldType Новый тип поля из перечисления.
     */
    void fieldTypeUpdate(int index, uint8_t newFieldType);
    /*!
     * \brief fieldPlayerNumberOwnerUpdate Сигнал обновления номера владельца поля.
     * \param index Настоящий индекс поля на сетке.
     * \param newPlayerNumberOwner Новый номер игрока из перечисления.
     */
    void fieldPlayerNumberOwnerUpdate(int index, uint8_t newPlayerNumberOwner);
    /*!
     * \brief fieldCostUpdate Сигнал обновления цены поля.
     * \param index Настоящий индекс поля на сетке.
     * \param newFieldCost Новая цена поля.
     */
    void fieldCostUpdate(int index, int newFieldCost);
    /*!
     * \brief fieldIncomeUpdate Сигнал обновления дохода поля.
     * \param index Настоящий индекс поля на сетке.
     * \param newFieldIncome Новый доход поля.
     */
    void fieldIncomeUpdate(int index, int newFieldIncome);
    /*!
     * \brief fieldPiecesMaskUpdate Сигнал обновления маски расположения игровых фишек на поле.
     * \param index Настоящий индекс поля на сетке.
     * \param newPiecesOnCellMask Новая маска расположения игровых фишек на поле.
     */
    void fieldPiecesMaskUpdate(int index, uint8_t newPiecesOnCellMask);
    /*!
     * \brief fieldStageUpdate Сигнал обновления минимальной стадии поля.
     * \param index Настоящий индекс поля на сетке.
     * \param newStage Новый номер минимальной стадии, на которой физически появляется поле.
     */
    void fieldStageUpdate(int index, uint8_t newStage);
    /*!
     * \brief fieldArrowDirectionUpdate Сигнал обновления направления поля-стрелки.
     * \param index Настоящий индекс поля на сетке.
     * \param newDirection Новое направление "стрелки" из перечисления.
     */
    void fieldArrowDirectionUpdate(int index, uint8_t newDirection);
    /*!
     * \brief fieldBlankUntilStageUpdate Сигнал обновления номера стадии, на которой открывается истинный тип поля.
     * \param index Настоящий индекс поля на сетке.
     * \param newBlankUntilStage Новый номер стадии, на которой открывается истинный тип поля.
     */
    void fieldBlankUntilStageUpdate(int index, uint8_t newBlankUntilStage);
};

#endif // GAMETRANSMITTEROBJECT_H
