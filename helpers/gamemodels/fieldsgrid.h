#ifndef FIELDSGRID_H
#define FIELDSGRID_H

#include <QObject>
#include <QAbstractListModel>
#include <QMap>

#include <vector>

#include "helpers/gamemodels/gamestructs/gamestructs.h"

using std::vector;

class FieldsGrid : public QAbstractListModel
{
    Q_OBJECT
public:

    FieldsGrid(const int mapDimension = DEFAULT_MAP_DIMENSION,
               const vector<Cell> &mapContext = {},
               QObject* parent = nullptr);
    ~FieldsGrid();

private:
    void parseMap(const vector<Cell> mapContext);

private:
    vector<Cell> m_map;
    int m_mapDimension;
    int m_cellsCount;

    enum MapModelCellRole
    {
        OrderIndex,
        FieldType,
        PlayerNumberOwner,
        FieldCost,
        FieldIncome,
        PiecesOnCellMask
    };

    QMap<short, QString> mapModelCellRoleNames { {OrderIndex, "orderIndex"},
                                                 {FieldType, "fieldType"},
                                                 {PlayerNumberOwner, "playerNumberOwner"},
                                                 {FieldCost, "fieldCost"},
                                                 {FieldIncome, "fieldIncome"},
                                                 {PiecesOnCellMask, "piecesOnCellMask"} };

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // FIELDSGRID_H
