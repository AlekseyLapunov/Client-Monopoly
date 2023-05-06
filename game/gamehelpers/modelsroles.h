#ifndef MODELSROLES_H
#define MODELSROLES_H

#include <QString>
#include <QMap>

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
    BlankUntilStage
};

static QMap<short, QString> mapModelCellRoleNames { {OrderIndex, "orderIndex"},
                                                    {FieldType, "fieldType"},
                                                    {PlayerNumberOwner, "playerNumberOwner"},
                                                    {FieldCost, "fieldCost"},
                                                    {FieldIncome, "fieldIncome"},
                                                    {PiecesOnCellMask, "piecesOnCellMask"},
                                                    {Stage, "stage"},
                                                    {ArrowDirection, "arrowDirection"},
                                                    {BlankUntilStage, "blankUntilStage"} };

#endif // MODELSROLES_H
