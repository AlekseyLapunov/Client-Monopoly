#ifndef FIELDSGRIDMODEL_H
#define FIELDSGRIDMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QMap>

#include <vector>

#include "game/models/lists/cellslist.h"
#include "game/gamehelpers/modelsroles.h"
#include "game/gamehelpers/gamestructs.h"

using std::vector;

/*!
 * \brief FieldsGridModel Класс модели игрового поля для взаимодействия с приложением QML.
 */
class FieldsGridModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(CellsList *list READ list WRITE setList)

public:
    explicit FieldsGridModel(QObject* parent = nullptr);

    /*!
     * \brief list Метод возвращает указатель на используемый список клеток игрового поля.
     * \return Указатель на используемый список клеток игрового поля.
     */
    CellsList *list() const;
    /*!
     * \brief setList Метод устанавливает список клеток игрового поля.
     * \param list Указатель на список клеток игрового поля.
     */
    void setList(CellsList *list);

private:
    /*!
     * \brief m_cellsList Внутренний указатель на список клеток игрового поля.
     */
    CellsList *m_cellsList;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant &value, int role = Qt::DisplayRole) override;
    virtual QHash<int, QByteArray> roleNames() const override;
};

#endif // FIELDSGRIDMODEL_H
