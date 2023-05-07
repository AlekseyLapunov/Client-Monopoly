#include "playersinfomodel.h"

PlayersInfoModel::PlayersInfoModel(QObject* parent) :
                        QAbstractListModel(parent),
                        m_playerGameInfoList(nullptr)
{
}

PlayerGameInfoList *PlayersInfoModel::list() const
{
    return m_playerGameInfoList;
}

int PlayersInfoModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (parent.isValid() || !m_playerGameInfoList)
        return 0;

    return m_playerGameInfoList->items().size();
}

QVariant PlayersInfoModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || !m_playerGameInfoList)
    {
        return QVariant();
    }

    const PlayerGameInfo item = m_playerGameInfoList->items().at(index.row());

    switch (role)
    {
    case PlayersModelInfoRole::PlayerNumber:
        return QVariant(item.playerNumber);
    case PlayersModelInfoRole::DisplayableName:
        return QVariant(item.displayableName);
    case PlayersModelInfoRole::CurrentBalance:
        return QVariant(item.currentBalance);
    case PlayersModelInfoRole::PiecePositionOnOrderIndex:
        return QVariant(item.piecePositionOnOrderIndex);
    default:
        return QVariant();
    }

    return QVariant();
}

bool PlayersInfoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!m_playerGameInfoList)
        return false;

    PlayerGameInfo item = m_playerGameInfoList->items().at(index.row());

    switch (role)
    {
    case PlayersModelInfoRole::PlayerNumber:
        item.playerNumber = value.toInt();
        break;
    case PlayersModelInfoRole::DisplayableName:
        item.displayableName = value.toString();
        break;
    case PlayersModelInfoRole::CurrentBalance:
        item.currentBalance = value.toInt();
        break;
    case PlayersModelInfoRole::PiecePositionOnOrderIndex:
        item.piecePositionOnOrderIndex = value.toInt();
        break;
    default:
        return false;
    }

    if(m_playerGameInfoList->setItemAt(index.row(), item))
    {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

QHash<int, QByteArray> PlayersInfoModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    for(int i = 0; i < playersModelInfoRoleNames.size(); i++)
    {
        roles[PlayersModelInfoRole::PlayerNumber + i] = playersModelInfoRoleNames[PlayersModelInfoRole::PlayerNumber + i].toUtf8();
    }

    return roles;
}

void PlayersInfoModel::setList(PlayerGameInfoList *list)
{
    beginResetModel();

    if (m_playerGameInfoList)
        m_playerGameInfoList->disconnect(this);

    m_playerGameInfoList = list;

    if (m_playerGameInfoList) {
        connect(m_playerGameInfoList, &PlayerGameInfoList::preItemAppended, this, [=]()
        {
            const int index = m_playerGameInfoList->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(m_playerGameInfoList, &PlayerGameInfoList::postItemAppended, this, [=]()
        {
            endInsertRows();
        });

        connect(m_playerGameInfoList, &PlayerGameInfoList::preItemRemoved, this, [=](int index)
        {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(m_playerGameInfoList, &PlayerGameInfoList::postItemRemoved, this, [=]()
        {
            endRemoveRows();
        });
    }

    endResetModel();
}
