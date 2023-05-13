#include "gamealgo.h"

static GameAlgorithm::Graph graph;

vector<int> GameAlgorithm::makeFullPath(vector<int> allPathPoints)
{
    if(allPathPoints.size() <= 1)
        return {};

    graph.formGraph();

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

    graph.clearGraph();

    return outputFullPath;
}

vector<int> GameAlgorithm::makePath(pair<int, int> pathPair)
{
    if(pathPair.first == pathPair.second)
        return {pathPair.second};

    vector<int> outputPath = {};

    Point firstPoint(pathPair.first);
    Point finalPoint(pathPair.second);

    if((graph.find(firstPoint) != nullptr) && (graph.find(finalPoint) != nullptr))
    {
        Vertex *cursor = graph.setCursorOnPoint(firstPoint.row, firstPoint.col);

        while ((cursor->carrier.row != finalPoint.row) || (cursor->carrier.col != finalPoint.col))
        {
            cursor = cursor->prev;
            outputPath.push_back(cursor->carrier.convertToIndex());
        }
    }
    else
    {
        Point cursorPoint = firstPoint;

        int rowCoeff = (firstPoint.row > finalPoint.row) ? -1 :
                       (firstPoint.row < finalPoint.row) ?  1 :
                                                            0 ;
        int colCoeff = (firstPoint.col > finalPoint.col) ? -1 :
                       (firstPoint.col < finalPoint.col) ?  1 :
                                                            0 ;

        while(cursorPoint != finalPoint)
        {
            cursorPoint.row += rowCoeff;
            cursorPoint.col += colCoeff;
            outputPath.push_back(cursorPoint.convertToIndex());
        }
    }

    return outputPath;
}
