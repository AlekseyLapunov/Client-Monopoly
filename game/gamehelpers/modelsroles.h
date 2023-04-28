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
    PiecesOnCellMask
};

static QMap<short, QString> mapModelCellRoleNames { {OrderIndex, "orderIndex"},
                                                    {FieldType, "fieldType"},
                                                    {PlayerNumberOwner, "playerNumberOwner"},
                                                    {FieldCost, "fieldCost"},
                                                    {FieldIncome, "fieldIncome"},
                                                    {PiecesOnCellMask, "piecesOnCellMask"} };

#endif // MODELSROLES_H
