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
    Stage
};

static QMap<short, QString> mapModelCellRoleNames { {OrderIndex, "orderIndex"},
                                                    {FieldType, "fieldType"},
                                                    {PlayerNumberOwner, "playerNumberOwner"},
                                                    {FieldCost, "fieldCost"},
                                                    {FieldIncome, "fieldIncome"},
                                                    {PiecesOnCellMask, "piecesOnCellMask"},
                                                    {Stage, "stage"} };

#endif // MODELSROLES_H
