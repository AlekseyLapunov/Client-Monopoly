#include "fieldsgridmodel.h"

FieldsGridModel::FieldsGridModel(QObject* parent) :
                        QAbstractListModel(parent),
                        m_cellsList(nullptr)
{
}

CellsList *FieldsGridModel::list() const
{
    return m_cellsList;
}

int FieldsGridModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (parent.isValid() || !m_cellsList)
        return 0;

    return m_cellsList->items().size();
}

QVariant FieldsGridModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || !m_cellsList)
    {
        return QVariant();
    }

    const Cell item = m_cellsList->items().at(index.row());

    switch (role)
    {
    case MapModelCellRole::OrderIndex:
        return QVariant(item.orderIndex);
    case MapModelCellRole::FieldType:
        return QVariant(item.fieldTypeSet);
    case MapModelCellRole::PlayerNumberOwner:
        return QVariant(item.playerNumberOwner);
    case MapModelCellRole::FieldCost:
        return QVariant(item.fieldCost);
    case MapModelCellRole::FieldIncome:
        return QVariant(item.fieldIncome);
    case MapModelCellRole::PiecesOnCellMask:
        return QVariant(item.piecesOnCellMask);
    case MapModelCellRole::Stage:
        return QVariant(item.stage);
    case MapModelCellRole::ArrowDirection:
        return QVariant(item.arrowDirection);
    case MapModelCellRole::BlankUntilStage:
        return QVariant(item.blankUntilStage);
    default:
        return QVariant();
    }

    return QVariant();
}

bool FieldsGridModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!m_cellsList)
        return false;

    Cell item = m_cellsList->items().at(index.row());

    switch (role)
    {
    case MapModelCellRole::OrderIndex:
        item.orderIndex = value.toInt();
        break;
    case MapModelCellRole::FieldType:
        item.fieldTypeSet = value.toInt();
        break;
    case MapModelCellRole::PlayerNumberOwner:
        item.playerNumberOwner = value.toInt();
        break;
    case MapModelCellRole::FieldCost:
        item.fieldCost = value.toInt();
        break;
    case MapModelCellRole::FieldIncome:
        item.fieldIncome = value.toInt();
        break;
    case MapModelCellRole::PiecesOnCellMask:
        item.piecesOnCellMask = value.toInt();
        break;
    case MapModelCellRole::Stage:
        item.stage = value.toInt();
        break;
    case MapModelCellRole::ArrowDirection:
        item.arrowDirection = value.toInt();
        break;
    case MapModelCellRole::BlankUntilStage:
        item.blankUntilStage = value.toInt();
        break;
    default:
        return false;
    }

    if(m_cellsList->setItemAt(index.row(), item))
    {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

QHash<int, QByteArray> FieldsGridModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    for(int i = 0; i < mapModelCellRoleNames.size(); i++)
    {
        roles[MapModelCellRole::OrderIndex + i] = mapModelCellRoleNames[MapModelCellRole::OrderIndex + i].toUtf8();
    }

    return roles;
}

void FieldsGridModel::setList(CellsList *list)
{
    beginResetModel();

    if (m_cellsList)
        m_cellsList->disconnect(this);

    m_cellsList = list;

    if (m_cellsList) {
        connect(m_cellsList, &CellsList::preItemAppended, this, [=]()
        {
            const int index = m_cellsList->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(m_cellsList, &CellsList::postItemAppended, this, [=]()
        {
            endInsertRows();
        });

        connect(m_cellsList, &CellsList::preItemRemoved, this, [=](int index)
        {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(m_cellsList, &CellsList::postItemRemoved, this, [=]()
        {
            endRemoveRows();
        });
    }

    endResetModel();
}
