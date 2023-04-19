import QtQuick 2.15
import QtQuick.Controls

Rectangle
{
    id: root
    property double mapScale: 3.1

    color: "transparent"

    GridView
    {
        id: _grid
        width: root.width - 2*root.border.width
        height: root.height - 2*root.border.width
        anchors.centerIn: root
        interactive: false

        cellHeight: _grid.width/Math.sqrt(_cellsList.count)
        cellWidth: cellHeight

        model: _cellsList
        delegate: Cell
        {
            height: _grid.cellHeight
            width: _grid.cellWidth
            border.color: root.border.color
            border.width: root.border.width/2
            fieldType: fieldTypeSet
        }
    }

    function refresh()
    {
        for(let i = 0; i < _cellsList.count; i++)
            _cellsList.get(i).fieldTypeSet = Math.floor((Math.random() * 11 + 1));
    }

    function addField()
    {
        let adding = Math.pow(_win.debugCellsCount, 2) - _cellsList.count;
        for(let i = 0; i < adding; i++)
        {
            let cellItem = {};
            cellItem.fieldTypeSet = Math.floor((Math.random() * 11 + 1));
            _cellsList.append(cellItem);
        }
    }

    function delField()
    {
        let deleting = _cellsList.count - Math.pow(_win.debugCellsCount, 2);
        for(let i = 0; i < deleting; i++)
        {
            if(_cellsList.count != 0)
                _cellsList.remove(_cellsList.count - 1);
        }
    }

    ListModel
    {
        id: _cellsList
    }
}
