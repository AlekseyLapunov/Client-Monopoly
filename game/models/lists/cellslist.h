#ifndef CELLSLIST_H
#define CELLSLIST_H

#include <vector>

#include <QObject>

#include "game/gamehelpers/gamestructs.h"

class CellsList : public QObject
{
    Q_OBJECT
public:
    explicit CellsList(QObject *parent = nullptr);

    void setItems(const std::vector<Cell> &newItems);
    void setItems(const CellsList &newItems);

    std::vector<Cell> items() const;

    bool setItemAt(int index, const Cell &item);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem();
    void appendItem(Cell &item);

private:
    std::vector<Cell> m_items;
};

#endif // CELLSLIST_H
