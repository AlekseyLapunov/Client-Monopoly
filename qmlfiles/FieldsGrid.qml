import QtQuick 2.15

Rectangle
{
    id: root
    property double mapScale: 2.6

    color: "transparent"

    GridView
    {
        property int cellsCount: 121

        id: _grid
        anchors.fill: parent
        model: cellsCount

        interactive: false

        cellHeight: root.width/Math.sqrt(cellsCount)
        cellWidth: cellHeight

        delegate: Cell
        {
            height: _grid.cellHeight
            width: _grid.cellWidth
            color: "white"
        }
    }
}
