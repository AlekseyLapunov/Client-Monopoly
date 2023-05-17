#ifndef MODELSROLES_H
#define MODELSROLES_H

#include <QString>
#include <QMap>

/*!
 * \brief MapModelCellRole Перечисление ролей для модели клетки игрового поля.
 */
enum MapModelCellRole
{
    OrderIndex = Qt::UserRole,
    FieldType,
    PlayerNumberOwner,
    FieldCost,
    FieldIncome,
    PiecesOnCellMask,
    Stage,
    ArrowDirection,
    BlankUntilStage,
    AllowedDirectionsMask
};

/*!
 * \brief PlayersModelInfoRole Перечисление ролей для модели игрока.
 */
enum PlayersModelInfoRole
{
    PlayerNumber = Qt::UserRole,
    DisplayableName,
    CurrentBalance,
    PiecePositionOnOrderIndex
};

static QMap<short, QString> mapModelCellRoleNames { {OrderIndex, "orderIndex"},
                                                    {FieldType, "fieldType"},
                                                    {PlayerNumberOwner, "playerNumberOwner"},
                                                    {FieldCost, "fieldCost"},
                                                    {FieldIncome, "fieldIncome"},
                                                    {PiecesOnCellMask, "piecesOnCellMask"},
                                                    {Stage, "stage"},
                                                    {ArrowDirection, "arrowDirection"},
                                                    {BlankUntilStage, "blankUntilStage"},
                                                    {AllowedDirectionsMask, "allowedDirectionsMask"} };

static QMap<short, QString> playersModelInfoRoleNames { {PlayerNumber, "playerNumber"},
                                                        {DisplayableName, "displayableName"},
                                                        {CurrentBalance, "currentBalance"},
                                                        {PiecePositionOnOrderIndex, "piecePositionOnOrderIndex"} };

#endif // MODELSROLES_H
