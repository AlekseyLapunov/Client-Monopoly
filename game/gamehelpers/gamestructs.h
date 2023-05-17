#ifndef GAMESTRUCTS_H
#define GAMESTRUCTS_H

#include <memory>
#include <stdint.h>
#include <vector>

#include "QString"

using std::vector;

#define DEFAULT_MAP_DIMENSION 9

/*!
 * \brief Cell Структура клетки игрового поля (карты).
 */
struct Cell
{
    bool operator==(const Cell &cellContext);
    bool operator==(Cell &cellContext);

    int orderIndex; //!< Порядковый номер клетки.
    uint8_t fieldTypeSet; //!< Тип поля из перечисления.
    uint8_t playerNumberOwner; //!< Номер игрока из перечисления.
    int fieldCost; //!< Цена поля в млн. ЭК.
    int fieldIncome; //!< Доходность поля в млн. ЭК.
    uint8_t piecesOnCellMask; //!< Битовая маска находящихся на клетке фишек игроков.
    uint8_t stage; //!< Стадия, на которой поле физически появляется (по клетке можно пройти).
    uint8_t arrowDirection; //!< Направление поля-стрелки. Игнорируется, если поле не "стрелка".
    uint8_t blankUntilStage; //!< Стадия, на которой поле раскрывает свой истинный тип.
    uint8_t allowedDirectionsMask; //!< Битовая маска разрешённых направлений поворота для поля-стрелки. Игнорируется, если поле не "стрелка".
};

/*!
* \brief PlayerGameInfo Структура игрока
*/
struct PlayerGameInfo
{
    bool operator==(const PlayerGameInfo &cellContext);
    bool operator==(PlayerGameInfo &cellContext);

    uint8_t playerNumber; //!< Номер игрока из перечисления.
    QString displayableName; //!< Отображаемый псевдоним игрока.
    int currentBalance; //!< Текущее количество ЭК в млн.
    int piecePositionOnOrderIndex; //!< Порядковый номер клетки, на которой в данный момент стоит фишка игрока.
};

/*!
 * \brief FieldType Перечисление типов клеток игрового поля (карты).
 */
enum FieldType
{
    Void, Common, Beginning, Sawmill, CoalStation,
    AtomicStation, Forest, Coal, Uranium, Vacation,
    Sabotage, Arrow
};

/*!
 * \brief PlayerNumber Перечисление номеров игроков. Отражает порядок хода.
 */
enum PlayerNumber
{
    NoPlayer, Player1, Player2, Player3,
    Player4, Player5, Player6
};

/*!
 * \brief Dice Перечисление игровых костей.
 */
enum Dice
{
    Left, Right
};

/*!
 * \brief ArrowDirection Перечисление направлений поворота поля-стрелки.
 */
enum ArrowDirection
{
    ArrowUp, ArrowRight, ArrowDown, ArrowLeft
};

#endif // GAMESTRUCTS_H
