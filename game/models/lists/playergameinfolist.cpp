#include "playergameinfolist.h"


PlayerGameInfoList::PlayerGameInfoList(QObject *parent) : QObject(parent)
{
}

void PlayerGameInfoList::setItems(const std::vector<PlayerGameInfo> &newItems)
{
    m_items.clear();
    m_items.resize(newItems.size());
    for(int i = 0; i < (int)newItems.size(); i++)
    {
        m_items[i] = newItems.at(i);
    }
}

void PlayerGameInfoList::setItems(const PlayerGameInfoList &newItems)
{
    m_items.clear();
    m_items.resize(newItems.m_items.size());
    for(int i = 0; i < (int)newItems.m_items.size(); i++)
    {
        m_items[i] = newItems.m_items[i];
    }
}

std::vector<PlayerGameInfo> PlayerGameInfoList::items() const
{
    return m_items;
}

PlayerGameInfo PlayerGameInfoList::getItemAt(int index)
{
    if (index < 0 || index >= (int)m_items.size())
        return {};

    return m_items.at(index);
}

bool PlayerGameInfoList::setItemAt(int index, const PlayerGameInfo &item)
{
    if (index < 0 || index >= (int)m_items.size())
        return false;

    PlayerGameInfo &oldItem = m_items.at(index);
    if (oldItem == item)
        return false;

    m_items[index] = item;
    return true;
}

void PlayerGameInfoList::sortByBalance()
{
    if(m_items.size() < 2)
        return;

    for (int i = 0; i < ((int)m_items.size() - 1); i++)
        for (int j = 0; j < ((int)m_items.size() - i - 1); j++)
            if (m_items[j].currentBalance < m_items[j + 1].currentBalance)
                std::swap(m_items[j], m_items[j+1]);
}

void PlayerGameInfoList::appendItem()
{
    emit preItemAppended();

    PlayerGameInfo item;
    item.playerNumber = NoPlayer;
    item.currentBalance = 0;
    item.displayableName = "NoPlayer";
    item.piecePositionOnOrderIndex = 0;
    m_items.push_back(item);

    emit postItemAppended();
}

void PlayerGameInfoList::appendItem(PlayerGameInfo &item)
{
    emit preItemAppended();

    m_items.push_back(item);

    emit postItemAppended();
}

void PlayerGameInfoList::removeItem()
{
    emit preItemRemoved(m_items.size() - 1);

    m_items.pop_back();

    emit postItemRemoved();
}

void PlayerGameInfoList::removeItem(int index)
{
    emit preItemRemoved(m_items.size() - 1);

    m_items.erase(m_items.begin()+index);

    emit postItemRemoved();
}
