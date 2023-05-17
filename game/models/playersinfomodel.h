#ifndef PLAYERSINFOMODEL_H
#define PLAYERSINFOMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QMap>

#include <vector>

#include "game/models/lists/playergameinfolist.h"
#include "game/gamehelpers/modelsroles.h"
#include "game/gamehelpers/gamestructs.h"

using std::vector;

/*!
 * \brief PlayersInfoModel Класс модели игроков для взаимодействия с приложением QML.
 */
class PlayersInfoModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(PlayerGameInfoList *list READ list WRITE setList)

public:
    explicit PlayersInfoModel(QObject* parent = nullptr);

    /*!
     * \brief list Метод возвращает указатель на используемый список игроков.
     * \return Указатель на используемый список игроков.
     */
    PlayerGameInfoList *list() const;
    /*!
     * \brief setList Метод устанавливает список игроков.
     * \param list Указатель на список игроков.
     */
    void setList(PlayerGameInfoList *list);

private:
    /*!
     * \brief m_cellsList Внутренний указатель на список игроков.
     */
    PlayerGameInfoList *m_playerGameInfoList;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant &value, int role = Qt::DisplayRole) override;
    virtual QHash<int, QByteArray> roleNames() const override;
};

#endif // PLAYERSINFOMODEL_H
