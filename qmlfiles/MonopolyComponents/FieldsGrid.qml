import QtQuick 2.15
import QtQuick.Controls
import "SubComponents"
import "../HelperSingletone"

Rectangle
{
    id: root
    property double mapScale: 3.1
    property color sharedColor: Qt.rgba(0, 0, 0, 0)
    property color baseColor: Qt.rgba(sharedColor.r-0.20,
                                      sharedColor.g-0.20,
                                      sharedColor.b-0.20)

    color: "transparent"
    border.color: Qt.darker(baseColor, 1)
    border.width: 3

    signal placeNewPieceSig(int cellId, int playerNumber)
    signal removeLastPieceSig(int cellId)

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
            border.width: root.border.width/3
            fieldType: fieldTypeSet
            cellNumber: index
            //visible: index%2

            Connections
            {
                target: root
                function onPlaceNewPieceSig(cellId, playerNumber) { placeNewPiece(cellId, playerNumber); }
                function onRemoveLastPieceSig(cellId) { removeLastPiece(cellId); }
            }

            PropertyAnimation on opacity
            {
                from: 0.0
                to: 1.0
                duration: 200
            }
        }
    }

    Rectangle
    {
        id: _overlayRectangle
        color: Qt.lighter(root.baseColor, 1.1)
        anchors.fill: root
        opacity: 0.1
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
            if(_cellsList.count !== 0)
            {
                _cellsList.remove(_cellsList.count - 1);
            }
        }
    }

    function resetField()
    {
        _cellsList.clear();
        for(let i = 0; i < _win.debugCellsCount; i++)
            addField();
    }

    function placeGamingPiece(cellId, playerNumberToPlace)
    {
        placeNewPieceSig(cellId, playerNumberToPlace);
    }

    function placeRandomPiece(cellId)
    {
        placeNewPieceSig(cellId, (Math.random()*Helper.PlayerNumber.Player6 + 1));
    }

    function delLastPiece(cellId)
    {
        removeLastPieceSig(cellId);
    }

    ListModel
    {
        id: _cellsList
    }
}
