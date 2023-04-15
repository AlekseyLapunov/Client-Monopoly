import QtQuick 2.15

Rectangle
{
    id: root
    property double mapScale: 3.4

    GridView
    {
        id: _grid
        anchors.fill: parent
        model: 121

        interactive: false

        cellHeight: root.width*0.09
        cellWidth: cellHeight

        delegate: Cell
        {
            height: _grid.cellHeight
            width: _grid.cellWidth
        }
    }
}
