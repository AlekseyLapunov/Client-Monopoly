#ifndef PLAYERGAMEINFOLIST_H
#define PLAYERGAMEINFOLIST_H

#include <vector>

#include <QObject>

#include "game/gamehelpers/gamestructs.h"

class PlayerGameInfoList : public QObject
{
    Q_OBJECT
public:
    explicit PlayerGameInfoList(QObject *parent = nullptr);

    void setItems(const std::vector<PlayerGameInfo> &newItems);
    void setItems(const PlayerGameInfoList &newItems);

    std::vector<PlayerGameInfo> items() const;

    PlayerGameInfo getItemAt(int index);
    bool setItemAt(int index, const PlayerGameInfo &item);

    void sortByBalance();

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem();
    void appendItem(PlayerGameInfo &item);
    void removeItem();
    void removeItem(int index);

private:
    std::vector<PlayerGameInfo> m_items;
};

#endif // PLAYERGAMEINFOLIST_H
