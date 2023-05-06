#include "cellslist.h"


CellsList::CellsList(QObject *parent) : QObject(parent)
{
}

void CellsList::setItems(const std::vector<Cell> &newItems)
{
    m_items.clear();
    m_items.resize(newItems.size());
    for(int i = 0; i < (int)newItems.size(); i++)
    {
        m_items[i] = newItems.at(i);
    }
}

void CellsList::setItems(const CellsList &newItems)
{
    m_items.clear();
    m_items.resize(newItems.m_items.size());
    for(int i = 0; i < (int)newItems.m_items.size(); i++)
    {
        m_items[i] = newItems.m_items[i];
    }
}

std::vector<Cell> CellsList::items() const
{
    return m_items;
}

Cell CellsList::getItemAt(int index)
{
    if (index < 0 || index >= (int)m_items.size())
        return {};

    return m_items.at(index);
}

bool CellsList::setItemAt(int index, const Cell &item)
{
    if (index < 0 || index >= (int)m_items.size())
        return false;

    Cell &oldItem = m_items.at(index);
    if (oldItem == item)
        return false;

    m_items[index] = item;
    return true;
}

void CellsList::appendItem()
{
    emit preItemAppended();

    Cell item;
    m_items.push_back(item);

    emit postItemAppended();
}

void CellsList::appendItem(Cell &item)
{
    emit preItemAppended();

    m_items.push_back(item);

    emit postItemAppended();
}
