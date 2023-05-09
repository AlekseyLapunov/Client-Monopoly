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

    int findIndexByPlayerNumber(uint8_t inputPlayerNumber);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem(uint8_t playerNumber, QString playerNickname,
                    int playerBalance, int pieceOnOrderPosition);
    void removeItem(uint8_t playerNumber);

private:
    std::vector<PlayerGameInfo> m_items;
};

#endif // PLAYERGAMEINFOLIST_H
