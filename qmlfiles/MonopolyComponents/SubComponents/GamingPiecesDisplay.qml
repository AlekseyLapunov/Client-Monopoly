import QtQuick 2.15
import "../../HelperSingletone"

Rectangle
{
    id: root
    color: "transparent"
    anchors.fill: parent

    Repeater
    {
        id: _gamingPiecesRepeater
        model: _piecesOnCell
        delegate: GamingPiece
        {
            anchors.centerIn: root
            sourceSize.height: root.height/1.1
            sourceSize.width: root.width/1.1
            whatPlayer: pieceNumber
        }
    }

    function addPiece(playerNumber)
    {
        let player = {};
        player.pieceNumber = playerNumber;
        _piecesOnCell.append(player);
    }

    function removePiece(playerNumber)
    {
        if(_piecesOnCell.count === 0)
            return;

        for(let i = 0; i < _piecesOnCell.count; i++)
        {
            if(_piecesOnCell.get(i).pieceNumber === playerNumber)
            {
                _piecesOnCell.remove(i);
                return;
            }
        }
    }

    function removeLastPiece()
    {
        if(_piecesOnCell.count === 0)
            return;

        _piecesOnCell.remove(_piecesOnCell.count - 1);
    }

    ListModel
    {
        id: _piecesOnCell

    }
}
