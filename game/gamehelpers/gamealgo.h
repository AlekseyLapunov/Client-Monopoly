#ifndef GAMEALGO_H
#define GAMEALGO_H

#include <vector>

#include <QDebug>
#include <QString>

#include "gamestructs.h"

using std::vector;
using std::pair;

/*!
 * \brief GameAlgorithm Класс с алгоритмами для игры.
 *
 * Данный класс содержит полезные алгоритмы для
 * внедрения в игровой процесс в качестве статических
 * методов.
 */
class GameAlgorithm
{
public:
    /*!
     * \brief makeFullPath Функция возвращает полный путь игровой фишки на основе заданных точек.
     * \param allPathPoints Входящие точки для отрисовки.
     * \return Полный путь фишки как вектор целочисленных значений.
     */
    static vector<int> makeFullPath(vector<int> allPathPoints);

private:
    /*!
     * \brief makePath Функция возвращает локальный путь игровой фишки на основе заданной пары точек.
     * \param pathPair Входящая пара точек.
     * \return Локальный путь фишки как вектор целочисленных значений.
     */
    static vector<int> makePath(pair<int, int> pathPair);

public:
    /*!
     * \brief Point Структура точки
     *
     * Точка представляет собой структуру, содержащую поля строки, столбца и базового делителя base
     * для определения row и col.
     */
    struct Point
    {
        int row;
        int col;
        int base;

        Point(int index, int setBase = 9)
        {
            this->row  = index/setBase;
            this->col  = index%setBase;
            this->base = setBase;
        }

        /*!
         * \brief convertToIndex Функция рассчитывает индекс в одномерном массиве на основе двух индексов двумерного.
         * \return Индекс точки в одномерном массиве.
         */
        int convertToIndex()
        {
            return row*base + col;
        }

        bool operator==(Point& other)
        {
            return ((this->row == other.row)&&(this->col == other.col));
        }

        bool operator!=(Point& other)
        {
            return !((*this) == other);
        }

        /*!
         * \brief rowOnMinBound Функция определяет, не находится ли точка на минимальной границе по строке.
         * \return true, если точка находится на минимальной границе по строке.
         */
        bool rowOnMinBound()
        {
            return (row == 0);
        }

        /*!
         * \brief rowOnMaxBound Функция определяет, не находится ли точка на максимальной границе по строке.
         * \return true, если точка находится на максимальной границе по строке.
         */
        bool rowOnMaxBound()
        {
            return (row == (base - 1));
        }

        /*!
         * \brief colOnMinBound Функция определяет, не находится ли точка на минимальной границе по столбцу.
         * \return true, если точка находится на минимальной границе по столбцу.
         */
        bool colOnMinBound()
        {
            return (col == 0);
        }

        /*!
         * \brief colOnMaxBound Функция определяет, не находится ли точка на максимальной границе по столбцу.
         * \return true, если точка находится на максимальной границе по столбцу.
         */
        bool colOnMaxBound()
        {
            return (col == (base - 1));
        }
    };

    /*!
     * \brief Vertex Структура вершины графа
     *
     * Представляет собой узел двусвязного списка. В качестве содержимых данных переносит объект структуры точки.
     */
    struct Vertex
    {
        Point carrier;
        Vertex *clockWiseLink;
        Vertex *counterClockWiseLink;

        Vertex(Point inputCarrier): carrier(inputCarrier), clockWiseLink(nullptr),
                                    counterClockWiseLink(nullptr) {};
    };

    /*!
     * \brief Graph Структура графа
     *
     * Представляет собой двусвязный список, узлы которого - объекты структуры Vertex вершины графа.
     */
    struct Graph
    {
        Vertex *first = nullptr;

        /*!
         * \brief formGraph Функция строит граф из вершин, назначая им координаты согласно форме карты игры.
         */
        void formGraph()
        {
            if(first != nullptr)
                this->clearGraph();

            Point pointIterator(0);

            Vertex *firstVertex = new Vertex(pointIterator);

            this->first = firstVertex;
            pointIterator.row = 1;
            pointIterator.col = 0;

            Vertex *lastVertex = firstVertex;

            enum MovingMode { DecreasingRow, IncreasingRow,
                              DecreasingCol, IncreasingCol };

            uint8_t movingMode = IncreasingRow;

            while((lastVertex->carrier.row != 0) || (lastVertex->carrier.col != 1))
            {
                Vertex *vertex = new Vertex(pointIterator);
                vertex->clockWiseLink = lastVertex;
                lastVertex->counterClockWiseLink = vertex;

                if(vertex->carrier.rowOnMinBound() && vertex->carrier.colOnMinBound())
                    movingMode = IncreasingRow;
                else if (vertex->carrier.rowOnMaxBound() && vertex->carrier.colOnMinBound())
                    movingMode = IncreasingCol;
                else if (vertex->carrier.rowOnMaxBound() && vertex->carrier.colOnMaxBound())
                    movingMode = DecreasingRow;
                else if (vertex->carrier.rowOnMinBound() && vertex->carrier.colOnMaxBound())
                    movingMode = DecreasingCol;

                switch (movingMode)
                {
                case IncreasingRow:
                    pointIterator.row++;
                    break;
                case IncreasingCol:
                    pointIterator.col++;
                    break;
                case DecreasingRow:
                    pointIterator.row--;
                    break;
                case DecreasingCol:
                    pointIterator.col--;
                    break;
                default:
                    break;
                }

                lastVertex = vertex;
            }

            first->clockWiseLink = lastVertex;
            lastVertex->counterClockWiseLink = first;

            lastVertex = nullptr;
        }

        /*!
         * \brief clearGraph Функция очищает и удаляет из памяти созданный граф.
         */
        void clearGraph()
        {
            Vertex *cursor = this->first;
            Vertex *remember = nullptr;

            do
            {
                remember = cursor;
                cursor = cursor->clockWiseLink;
                delete remember;
            } while(cursor != first);

            this->first = nullptr;
        }

        /*!
         * \brief setCursorOnPoint Функция определяет указатель на вершину со
         * совпадающими со входящими значениями строки и столбца точки.
         * \param row Номер строки, на которой находится точка.
         * \param col Номер столбца, на котором находится точка.
         * \return Указатель на вершину.
         */
        Vertex* setCursorOnPoint(int row, int col)
        {
            Vertex *cursor = this->first;

            do
            {
                if((cursor->carrier.row == row) && (cursor->carrier.col == col))
                {
                    return cursor;
                }

                cursor = cursor->clockWiseLink;
            } while(cursor != this->first);

            return nullptr;
        }

        /*!
         * \brief find Функция определяет указатель на вершину, точка которой совпадает со входящей точкой.
         * \param point Точка, которую нужно найти.
         * \return Указатель на вершину.
         */
        Vertex* find(Point point)
        {
            Vertex *cursor = this->first;

            do
            {
                if(cursor->carrier == point)
                {
                    return cursor;
                }

                cursor = cursor->clockWiseLink;
            } while(cursor != this->first);

            return nullptr;
        }

        /*!
         * \brief GraphWay Перечисление, отражающее путь по графу.
         */
        enum GraphWay { WayUndefined, WayClockWise, WayCounterClockWise };

        /*!
         * \brief distanceToPoint Функция определяет расстояние до точки графа.
         * \param point Точка, которую необходимо найти.
         * \param startingAt Указатель на вершину начала поиска.
         * \param graphWayFlag Флаг поиска "по часовой" или "против часовой" стрелке.
         * \return Расстояние до искомой точки.
         */
        int distanceToPoint(Point point, Vertex *startingAt, uint8_t graphWayFlag = GraphWay::WayClockWise)
        {
            int distance = 0;

            Vertex *cursor = startingAt;

            do
            {
                if(cursor->carrier == point)
                    return distance;

                switch (graphWayFlag)
                {
                case GraphWay::WayClockWise:
                    cursor = cursor->clockWiseLink;
                    break;
                case GraphWay::WayCounterClockWise:
                    cursor = cursor->counterClockWiseLink;
                    break;
                default:
                    return -1;
                }

                distance++;
            }
            while(cursor != startingAt);

            return -1;
        }

        /*!
         * \brief defineShortestWay Функция определяет по какому направлению дойти до точки графа будет ближе.
         * \param point Точка, до которой нужно дойти.
         * \param startingAt Указатель на вершину начала поиска.
         * \return Флаг поиска "по часовой" или "против часовой" стрелке.
         */
        uint8_t defineShortestWay(Point point, Vertex *startingAt)
        {
            int clockWiseDistance        = distanceToPoint(point, startingAt, GraphWay::WayClockWise);
            int counterClockWiseDistance = distanceToPoint(point, startingAt, GraphWay::WayCounterClockWise);

            if((clockWiseDistance == -1) || (counterClockWiseDistance == -1))
                return GraphWay::WayUndefined;

            if(clockWiseDistance < counterClockWiseDistance)
                return GraphWay::WayClockWise;
            else
                return GraphWay::WayCounterClockWise;
        }
    };
};

#endif // GAMEALGO_H
