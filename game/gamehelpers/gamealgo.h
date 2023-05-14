#ifndef GAMEALGO_H
#define GAMEALGO_H

#include <vector>

#include <QDebug>
#include <QString>

#include "gamestructs.h"

using std::vector;
using std::pair;

class GameAlgorithm
{
public:
    static vector<int> makeFullPath(vector<int> allPathPoints);

private:
    static vector<int> makePath(pair<int, int> pathPair);

public:
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

        bool rowOnMinBound()
        {
            return (row == 0);
        }

        bool rowOnMaxBound()
        {
            return (row == (base - 1));
        }

        bool colOnMinBound()
        {
            return (col == 0);
        }

        bool colOnMaxBound()
        {
            return (col == (base - 1));
        }
    };

    struct Vertex
    {
        Point carrier;
        Vertex *clockWiseLink;
        Vertex *counterClockWiseLink;

        Vertex(Point inputCarrier): carrier(inputCarrier), clockWiseLink(nullptr),
                                    counterClockWiseLink(nullptr) {};
    };

    struct Graph
    {
        Vertex *first = nullptr;

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

        enum GraphWay { WayUndefined, WayClockWise, WayCounterClockWise };

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
