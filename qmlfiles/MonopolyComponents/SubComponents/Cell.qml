import QtQuick 2.15
import Qt5Compat.GraphicalEffects
import "../../HelperSingletone"

Rectangle
{
    id: root

    property string backgroundColor: "transparent"
    property int fieldType: Helper.FieldType.Void
    property int cellNumber: -1

    visible: fieldType === Helper.FieldType.Void ? false : true
    color: Helper.defineFieldColorByType(fieldType)

    Image
    {
        id: _ownerFrame
        anchors.fill: root
    }

    Image
    {
        id: _fieldTypeImage
        anchors.centerIn: root
        source: Helper.defineFieldImageByType(fieldType)
        smooth: true
        sourceSize.width: root.width*0.9
        sourceSize.height: root.height*0.9
    }

    GamingPiecesDisplay
    {
        id: _gamingPiecesDisplay
        anchors.fill: root
    }

    function placeNewPiece(cellId, playerNumber)
    {
        if(root.cellNumber !== cellId)
            return;
        _gamingPiecesDisplay.addPiece(playerNumber);
    }

    function removeLastPiece(cellId)
    {
        if(root.cellNumber !== cellId)
            return;
        _gamingPiecesDisplay.removeLastPiece();
    }
}
