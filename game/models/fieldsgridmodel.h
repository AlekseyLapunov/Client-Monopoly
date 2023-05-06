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

class FieldsGridModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(CellsList *list READ list WRITE setList NOTIFY listChanged)

public:
    explicit FieldsGridModel(QObject* parent = nullptr);

    CellsList *list() const;
    void setList(CellsList *list);

signals:
    void listChanged(CellsList* list);

private:
    CellsList *m_cellsList;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant &value, int role = Qt::DisplayRole) override;
    virtual QHash<int, QByteArray> roleNames() const override;
};

#endif // FIELDSGRIDMODEL_H
