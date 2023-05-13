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
    static vector<int> makeFullPath(vector<int> allPathPoints)
    {
        if(allPathPoints.size() <= 1)
            return {};

        vector<int> outputFullPath = {};

        size_t iter = 0;

        while(iter < allPathPoints.size() - 1)
        {
           vector<int> localPath = makePath({allPathPoints.at(iter),
                                             allPathPoints.at(iter+1)});

           for(size_t i = 0; i < localPath.size(); i++)
               outputFullPath.push_back(localPath.at(i));

           iter++;
        }

        return outputFullPath;
    }

private:
    static vector<int> makePath(pair<int, int> pathPair)
    {
        if(pathPair.first == pathPair.second)
            return {pathPair.second};

        vector<int> outputPath = {};

        Point firstPoint(pathPair.first);
        Point secondPoint(pathPair.second);

        Point cursorPoint = firstPoint;

        int rowCoeff = (firstPoint.row > secondPoint.row) ? -1 :
                       (firstPoint.row < secondPoint.row) ?  1 :
                                                             0 ;
        int colCoeff = (firstPoint.col > secondPoint.col) ? -1 :
                       (firstPoint.col < secondPoint.col) ?  1 :
                                                             0 ;

        while(cursorPoint != secondPoint)
        {
            if((rowCoeff == 0) || (colCoeff == 0))
            {
                cursorPoint.row += rowCoeff;
                cursorPoint.col += colCoeff;
            }
            else if(((rowCoeff == 1) && (cursorPoint.rowOnMinBound())
                     && ((colCoeff == -1) && (!cursorPoint.colOnMinBound()))) ||
                    ((rowCoeff == -1) && (cursorPoint.rowOnMinBound())
                     && ((colCoeff == 1) && (!cursorPoint.colOnMaxBound()))) ||
                    ((rowCoeff == 1) && (cursorPoint.rowOnMinBound())
                     && ((colCoeff == 1) && (!cursorPoint.colOnMaxBound()))) ||
                    ((rowCoeff == -1) && (cursorPoint.rowOnMinBound())
                     && ((colCoeff == -1) && (!cursorPoint.colOnMinBound()))) ||
                    ((rowCoeff == 1) && (cursorPoint.rowOnMaxBound())
                     && ((colCoeff == 1) && (!cursorPoint.colOnMaxBound()))) ||
                    ((rowCoeff == -1) && (cursorPoint.rowOnMaxBound())
                     && ((colCoeff == -1) && (!cursorPoint.colOnMinBound()))) ||
                    ((rowCoeff == 1) && (cursorPoint.rowOnMaxBound())
                     && ((colCoeff == -1) && (!cursorPoint.colOnMinBound()))) ||
                    ((rowCoeff == -1) && (cursorPoint.rowOnMaxBound())
                     && ((colCoeff == 1) && (!cursorPoint.colOnMaxBound()))))
            {
                cursorPoint.col += colCoeff;
            }
            else if(((rowCoeff == 1) && (!cursorPoint.rowOnMaxBound())
                     && ((colCoeff == -1) && (cursorPoint.colOnMinBound()))) ||
                    ((rowCoeff == -1) && (!cursorPoint.rowOnMinBound())
                     && ((colCoeff == 1) && (cursorPoint.colOnMinBound()))) ||
                    ((rowCoeff == 1) && (!cursorPoint.rowOnMaxBound())
                     && ((colCoeff == 1) && (cursorPoint.colOnMaxBound()))) ||
                    ((rowCoeff == -1) && (!cursorPoint.rowOnMinBound())
                     && ((colCoeff == -1) && (cursorPoint.colOnMaxBound()))) ||
                    ((rowCoeff == 1) && (!cursorPoint.rowOnMaxBound())
                     && ((colCoeff == 1) && (cursorPoint.colOnMinBound()))) ||
                    ((rowCoeff == 1) && (!cursorPoint.rowOnMaxBound())
                     && ((colCoeff == -1) && (cursorPoint.colOnMaxBound()))) ||
                    ((rowCoeff == -1) && (!cursorPoint.rowOnMinBound())
                     && ((colCoeff == -1) && (cursorPoint.colOnMinBound()))) ||
                    ((rowCoeff == -1) && (!cursorPoint.rowOnMinBound())
                     && ((colCoeff == 1) && (cursorPoint.colOnMaxBound()))))
            {
                cursorPoint.row += rowCoeff;
            }

            outputPath.push_back(cursorPoint.convertToIndex());
        }

        return outputPath;
    }

private:
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

        bool rowOnBound()
        {
            return ((row == 0) || (row == (base - 1)));
        }

        bool rowOnMinBound()
        {
            return (row == 0);
        }

        bool rowOnMaxBound()
        {
            return (row == (base - 1));
        }

        bool colOnBound()
        {
            return ((col == 0) || (col == (base - 1)));
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
};

#endif // GAMEALGO_H
