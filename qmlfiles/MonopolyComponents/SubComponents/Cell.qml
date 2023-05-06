import QtQuick 2.15
import Qt5Compat.GraphicalEffects
import "../../HelperSingletone"
import "../../MonopolyDialogs"

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
    property int arrowDirection: Helper.ArrowRotation.North
    property int blankUntilStage: 0

    property bool ignoreClickForDialog: false

    // On-grid number
    property int cellNumber: -1

    property int displayableFieldType: blankUntilStage > currentStage ? Helper.FieldType.Common : fieldType

    visible: ((displayableFieldType !== Helper.FieldType.Void)
              && (stage <= currentStage))
    color: Helper.defineFieldColorByType(displayableFieldType)

    Image
    {
        id: _ownerFrame
        visible: (playerNumberOwner !== Helper.PlayerNumber.NoPlayer)
        anchors.fill: root
        source: "../../../assets/svgs/fields/owner_frame.svg"

        ColorOverlay
        {
            id: _ownerFrameColorOverlay
            color: Helper.definePlayerColorByNumber(playerNumberOwner)
            source: _ownerFrame
            anchors.fill: _ownerFrame
        }

        z: 2
    }

    Image
    {
        id: _fieldTypeImage
        anchors.centerIn: root
        source: Helper.defineFieldImageByType(displayableFieldType)
        smooth: true
        sourceSize.width: root.width*0.9
        sourceSize.height: root.height*0.9

        rotation: Helper.defineRotationByFieldType(displayableFieldType, arrowDirection)

        Behavior on rotation
        {
            PropertyAnimation
            {
                easing.type: Easing.OutElastic
                duration: 500
            }
        }
    }

    GamingPiecesDisplay
    {
        id: _gamingPiecesDisplay
        anchors.fill: root
    }

    MouseArea
    {
        id: _mouseArea
        anchors.fill: root
        enabled: !ignoreClickForDialog
        cursorShape: (Helper.canFieldExecuteDialog(fieldType) && !ignoreClickForDialog) ? "PointingHandCursor"
                                                                                        : "ArrowCursor"
        onClicked:
        {
            if(Helper.canFieldExecuteDialog(fieldType) && !ignoreClickForDialog)
                _displayField.showCellDialog(root.fieldType, root.playerNumberOwner,
                                             root.fieldCost, root.fieldIncome)
        }
    }

    Connections
    {
        target: _gameTransmitter

        function onFieldChanged(index, orderIndex, fieldTypeSet, playerNumberOwner,
                                fieldCost, fieldIncome, piecesOnCellMask, stage,
                                arrowDirection, blankUntilStage)
        {
            if(index !== root.cellNumber)
                return;

            root.orderIndex = orderIndex;
            root.fieldType = fieldTypeSet;
            root.playerNumberOwner = playerNumberOwner;
            root.fieldCost = fieldCost;
            root.fieldIncome = fieldIncome;
            root.piecesOnCellMask = piecesOnCellMask;
            root.stage = stage;
            root.arrowDirection = arrowDirection;
            root.blankUntilStage = blankUntilStage;
        }

        function onFieldOrderIndexChanged(index, orderIndex)
        {
            if(index !== root.cellNumber)
                return;

            root.orderIndex = orderIndex;
        }
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
