import QtQuick 2.15
import Qt5Compat.GraphicalEffects
import "../../HelperSingletone"

Rectangle
{
    id: root

    property string backgroundColor: "transparent"

    // Model properties
    property int orderIndex: 0
    property int fieldType: Helper.FieldType.Void
    property int playerNumberOwner: Helper.PlayerNumber.NoPlayer
    property int fieldCost: 0
    property int fieldIncome: 0
    property int piecesOnCellMask: 0
    property int stage: 0

    // On-grid number
    property int cellNumber: -1

    visible: ((fieldType !== Helper.FieldType.Void)
              && (stage <= currentStage))
    color: Helper.defineFieldColorByType(fieldType)

    Image
    {
        id: _ownerFrame
        visible: (playerNumberOwner !== Helper.PlayerNumber.NoPlayer)
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
