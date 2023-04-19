import QtQuick 2.15

Rectangle
{
    id: root
    property double mapScale: 3.1

    color: "transparent"

    GridView
    {
        //property int cellsCount: 121
        property int cellsCount: 1

        id: _grid
        width: root.width - 2*root.border.width
        height: root.height - 2*root.border.width
        anchors.centerIn: root
        model: cellsCount

        interactive: false

        cellHeight: _grid.width/Math.sqrt(cellsCount)
        cellWidth: cellHeight

        delegate: Cell
        {
            height: _grid.cellHeight
            width: _grid.cellWidth
            border.color: root.border.color
            border.width: root.border.width/2
            fieldType: Math.floor((Math.random() * 11 + 1))
        }
    }
}
