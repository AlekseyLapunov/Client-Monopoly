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
            sourceSize.height: (_piecesOnCell.count === 1) ? (root.height) : (1.1*(root.height/Math.sqrt(_piecesOnCell.count)))
            sourceSize.width:  (_piecesOnCell.count === 1) ? (root.width)  : (1.1*(root.width/Math.sqrt(_piecesOnCell.count)))
            x: defineX(index, sourceSize.width)
            y: defineY(index, sourceSize.height)
            whatPlayer: pieceNumber

            PropertyAnimation on opacity
            {
                easing.type: Easing.InQuad
                from: 0.0
                to: 1.0
                duration: 250
            }

            Behavior on x { PropertyAnimation { easing.type: Easing.OutQuart; duration: 450} }
            Behavior on y { PropertyAnimation { easing.type: Easing.InQuart; duration: 450} }
        }
    }

    function addPiece(playerNumber)
    {
        if(_piecesOnCell.count === 6)
            return;

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

    function defineX(index, imageWidth)
    {
        if(_piecesOnCell.count === 0)
            return 0;

        if(index === undefined)
            return 0;

        let scaleUnit = function(coeff) { return (-1*imageWidth + imageWidth*coeff) };

        switch(_piecesOnCell.count)
        {
        case 1:
            return scaleUnit(1.0);
        case 2:
            switch(index)
            {
            case 0:
                return scaleUnit(0.86);
            case 1:
                return scaleUnit(1.44);
            default:
                return 0;
            }
        case 3:
            switch(index)
            {
            case 0:
                return scaleUnit(1.29);
            case 1:
                return scaleUnit(0.85);
            case 2:
                return scaleUnit(1.75);
            default:
                return 0;
            }
        case 4:
            switch(index)
            {
            case 0:
                return scaleUnit(1.00);
            case 1:
                return scaleUnit(1.82);
            case 2:
                return scaleUnit(1.00);
            case 3:
                return scaleUnit(1.82);
            default:
                return 0;
            }
        case 5:
            switch(index)
            {
            case 0:
                return scaleUnit(1.03);
            case 1:
                return scaleUnit(1.99);
            case 2:
                return scaleUnit(1.03);
            case 3:
                return scaleUnit(1.99);
            case 4:
                return scaleUnit(1.51);
            default:
                return 0;
            }
        case 6:
            switch(index)
            {
            case 0:
                return scaleUnit(1.06);
            case 1:
                return scaleUnit(1.62);
            case 2:
                return scaleUnit(2.18);
            case 3:
                return scaleUnit(1.06);
            case 4:
                return scaleUnit(1.62);
            case 5:
                return scaleUnit(2.18);
            default:
                return 0;
            }
        }
    }

    function defineY(index, imageHeight)
    {
        if(_piecesOnCell.count === 0)
            return 0;

        if(index === undefined)
            return 0;

        let scaleUnit = function(coeff) { return (-1*imageHeight + imageHeight*coeff) };

        switch(_piecesOnCell.count)
        {
        case 1:
            return scaleUnit(0.99);
        case 2:
            switch(index)
            {
            case 0:
                return scaleUnit(1.16);
            case 1:
                return scaleUnit(1.16);
            default:
                return 0;
            }
        case 3:
            switch(index)
            {
            case 0:
                return scaleUnit(0.95);
            case 1:
                return scaleUnit(1.58);
            case 2:
                return scaleUnit(1.58);
            default:
                return 0;
            }
        case 4:
            switch(index)
            {
            case 0:
                return scaleUnit(0.98);
            case 1:
                return scaleUnit(0.98);
            case 2:
                return scaleUnit(1.83);
            case 3:
                return scaleUnit(1.83);
            default:
                return 0;
            }
        case 5:
            switch(index)
            {
            case 0:
                return scaleUnit(1.08);
            case 1:
                return scaleUnit(1.08);
            case 2:
                return scaleUnit(1.93);
            case 3:
                return scaleUnit(1.93);
            case 4:
                return scaleUnit(1.52);
            default:
                return 0;
            }
        case 6:
            switch(index)
            {
            case 0:
                return scaleUnit(1.15);
            case 1:
                return scaleUnit(1.15);
            case 2:
                return scaleUnit(1.15);
            case 3:
                return scaleUnit(2.06);
            case 4:
                return scaleUnit(2.06);
            case 5:
                return scaleUnit(2.06);
            default:
                return 0;
            }
        }
    }
}
