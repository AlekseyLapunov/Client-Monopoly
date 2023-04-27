#include "fieldsgrid.h"

FieldsGrid::FieldsGrid(const int mapDimension,
                       const vector<Cell> &mapContext,
                       QObject* parent) :
    QAbstractListModel(parent)
{
    m_mapDimension = mapDimension;
    m_cellsCount = m_mapDimension*m_mapDimension;

    m_map.resize(m_cellsCount);
    parseMap(mapContext);
}

FieldsGrid::~FieldsGrid()
{
    m_map.clear();
}

int FieldsGrid::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_map.size();
}

QVariant FieldsGrid::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    const int rowIndex = index.row();

    role -= Qt::UserRole;

    switch (role)
    {
    case MapModelCellRole::OrderIndex:
        return QVariant::fromValue(m_map[rowIndex].orderIndex);
    case MapModelCellRole::FieldType:
        return QVariant::fromValue(m_map[rowIndex].fieldType);
    case MapModelCellRole::PlayerNumberOwner:
        return QVariant::fromValue(m_map[rowIndex].playerNumberOwner);
    case MapModelCellRole::FieldCost:
        return QVariant::fromValue(m_map[rowIndex].fieldCost);
    case MapModelCellRole::FieldIncome:
        return QVariant::fromValue(m_map[rowIndex].fieldIncome);
    case MapModelCellRole::PiecesOnCellMask:
        return QVariant::fromValue(m_map[rowIndex].piecesOnCellMask);
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> FieldsGrid::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    for(int i = 0; i < mapModelCellRoleNames.count(); i++)
    {
        roles.insert(Qt::UserRole + i, mapModelCellRoleNames[i].toUtf8());
    }

    qDebug() << roles;

    return roles;
}

void FieldsGrid::parseMap(const vector<Cell> mapContext)
{
    m_map = mapContext;
}
