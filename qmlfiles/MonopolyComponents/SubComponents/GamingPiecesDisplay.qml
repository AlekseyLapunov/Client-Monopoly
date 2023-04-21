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
            sourceSize.height: root.height/((_piecesOnCell.count+1)*0.49)
            sourceSize.width: root.width/((_piecesOnCell.count+1)*0.49)
            x: defineX(index, sourceSize.width)
            y: defineY(index, sourceSize.height)
            whatPlayer: pieceNumber
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
        switch(_piecesOnCell.count)
        {
        case 1:
            return -1*imageWidth + imageWidth*0.99;
        case 2:
            switch(index)
            {
            case 0:
                return root.width;
            case 1:
                return root.width;
            }
            break;
        case 3:
            switch(index)
            {
            case 0:
                return root.width;
            case 1:
                return root.width;
            case 2:
                return root.width;
            }
            break;
        case 4:
            switch(index)
            {
            case 0:
                return root.width;
            case 1:
                return root.width;
            case 2:
                return root.width;
            case 3:
                return root.width;
            }
            break;
        case 5:
            switch(index)
            {
            case 0:
                return root.width;
            case 1:
                return root.width;
            case 2:
                return root.width;
            case 3:
                return root.width;
            case 4:
                return root.width;
            }
            break;
        case 6:
            switch(index)
            {
            case 0:
                return root.width;
            case 1:
                return root.width;
            case 2:
                return root.width;
            case 3:
                return root.width;
            case 4:
                return root.width;
            case 5:
                return root.width;
            }
            break;
        }
    }

    function defineY(index, imageHeight)
    {
        if(_piecesOnCell.count === 0)
            return 0;
        switch(_piecesOnCell.count)
        {
        case 1:
            return -1*imageHeight + imageHeight*0.99;
        case 2:
            switch(index)
            {
            case 0:
                return root.height;
            case 1:
                return root.height;
            }
            break;
        case 3:
            switch(index)
            {
            case 0:
                return root.height;
            case 1:
                return root.height;
            case 2:
                return root.height;
            }
            break;
        case 4:
            switch(index)
            {
            case 0:
                return root.height;
            case 1:
                return root.height;
            case 2:
                return root.height;
            case 3:
                return root.height;
            }
            break;
        case 5:
            switch(index)
            {
            case 0:
                return root.height;
            case 1:
                return root.height;
            case 2:
                return root.height;
            case 3:
                return root.height;
            case 4:
                return root.height;
            }
            break;
        case 6:
            switch(index)
            {
            case 0:
                return root.height;
            case 1:
                return root.height;
            case 2:
                return root.height;
            case 3:
                return root.height;
            case 4:
                return root.height;
            case 5:
                return root.height;
            }
            break;
        }
    }
}
