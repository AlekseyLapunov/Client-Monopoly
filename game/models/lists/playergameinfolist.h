#ifndef PLAYERGAMEINFOLIST_H
#define PLAYERGAMEINFOLIST_H

#include <vector>

#include <QObject>

#include "game/gamehelpers/gamestructs.h"

/*!
 * \brief PlayerGameInfoList Класс списка игроков.
 */
class PlayerGameInfoList : public QObject
{
    Q_OBJECT
public:
    explicit PlayerGameInfoList(QObject *parent = nullptr);

    /*!
     * \brief setItems Метод устанавливает список игроков.
     * \param newItems Список игроков.
     */
    void setItems(const std::vector<PlayerGameInfo> &newItems);
    /*!
     * \brief setItems Метод устанавливает список игроков.
     * \param newItems Список игроков.
     */
    void setItems(const PlayerGameInfoList &newItems);

    /*!
     * \brief items Метод возвращает содержимое внутреннего контейнера.
     * \return Содержимое внутреннего контейнера.
     */
    std::vector<PlayerGameInfo> items() const;

    /*!
     * \brief getItemAt Метод возвращает объект структуры игрока.
     * \param index Индекс в списке.
     * \return Объект структуры игрока.
     */
    PlayerGameInfo getItemAt(int index);
    /*!
     * \brief setItemAt Метод устанавливает объект структуры игрока.
     * \param index Индекс установки.
     * \param item Объект структуры игрока для установки.
     * \return Флаг успеха. true, если установка прошла успешно, и false - если нет.
     */
    bool setItemAt(int index, const PlayerGameInfo &item);

    /*!
     * \brief sortByBalance Метод выполняет сортировку внутреннего контейнера по балансу игроков
     * в убывающем порядке.
     */
    void sortByBalance();

    /*!
     * \brief findIndexByOrderIndex Метод возвращает индекс из списка по входящему номеру игрока.
     * \param inputPlayerNumber Номер игрока.
     * \return Индекс из списка.
     */
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
