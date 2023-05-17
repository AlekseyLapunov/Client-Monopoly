#ifndef CELLSLIST_H
#define CELLSLIST_H

#include <vector>

#include <QObject>

#include "game/gamehelpers/gamestructs.h"

/*!
 * \brief CellsList Класс списка игровых клеток.
 */
class CellsList : public QObject
{
    Q_OBJECT
public:
    explicit CellsList(QObject *parent = nullptr);

    /*!
     * \brief setItems Метод устанавливает список клеток.
     * \param newItems Список клеток.
     */
    void setItems(const std::vector<Cell> &newItems);

    /*!
     * \brief setItems Метод устанавливает список клеток.
     * \param newItems Список клеток.
     */
    void setItems(const CellsList &newItems);

    /*!
     * \brief items Метод возвращает содержимое внутреннего контейнера.
     * \return Содержимое внутреннего контейнера.
     */
    std::vector<Cell> items() const;

    /*!
     * \brief getItemAt Метод возвращает объект структуры клетки игрового поля.
     * \param index Индекс в списке.
     * \return Объект структуры клетки игрового поля.
     */
    Cell getItemAt(int index);
    /*!
     * \brief setItemAt Метод устанавливает объект структуры клетки игрового поля.
     * \param index Индекс установки.
     * \param item Объект структуры клетки игрового поля для установки.
     * \return Флаг успеха. true, если установка прошла успешно, и false - если нет.
     */
    bool setItemAt(int index, const Cell &item);

    /*!
     * \brief findIndexByOrderIndex Метод возвращает индекс из списка по входящему порядковому индексу клетки.
     * \param inputOrderIndex Порядковый индекс клетки.
     * \return Индекс из списка.
     */
    int findIndexByOrderIndex(int inputOrderIndex);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem();
    void appendItem(Cell &item);

private:
    /*!
     * \brief m_items Внутренний контейнер клеток игрового поля.
     */
    std::vector<Cell> m_items;
};

#endif // CELLSLIST_H
