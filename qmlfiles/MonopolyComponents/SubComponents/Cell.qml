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
    property int arrowDirection: Helper.ArrowRotation.ArrowUp
    property int blankUntilStage: 0

    property bool ignoreClickForDialog: false

    // On-grid number
    property int cellNumber: -1

    property int displayableFieldType: blankUntilStage > currentStage ? Helper.FieldType.Common : fieldType
    property int lastPiecesMask: 0

    visible: ((displayableFieldType !== Helper.FieldType.Void)
              && (stage <= currentStage))
    color: Helper.defineFieldColorByType(displayableFieldType)

    Image
    {
        id: _ownerFrame
        visible: (playerNumberOwner !== Helper.PlayerNumber.NoPlayer)
        anchors.fill: root
        source: "../../../assets/svgs/fields/owner_frame.svg"

        smooth: true

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
            enabled: displayableFieldType === Helper.FieldType.Arrow
            RotationAnimator
            {
                target: _fieldTypeImage
                direction: RotationAnimator.Shortest
                easing.type: Easing.OutElastic
                duration: 1500
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
        enabled: (!ignoreClickForDialog && Helper.canFieldExecuteDialog(displayableFieldType))
        visible: enabled

        cursorShape: "PointingHandCursor"
        onClicked:
        {
            if(Helper.canFieldExecuteDialog(displayableFieldType) && !ignoreClickForDialog)
                _displayField.showCellDialog(root.displayableFieldType, root.playerNumberOwner,
                                             _playersInfoModel.getPlayerNicknameByNumber(root.playerNumberOwner),
                                             root.fieldCost, root.fieldIncome, root.arrowDirection)
        }
    }

    Component.onCompleted:
    {
        placePiecesAccordingToMask();
        lastPiecesMask = root.piecesOnCellMask;
    }

    onPiecesOnCellMaskChanged:
    {
        placePiecesAccordingToMask();
        lastPiecesMask = root.piecesOnCellMask;
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

        function onFieldPiecesMaskChanged(index, piecesMask)
        {
            if(index !== root.cellNumber)
                return;

            root.piecesOnCellMask = piecesMask;
        }
    }

    function placePiecesAccordingToMask()
    {
        let piecesLeftMask = Helper.definePiecesLeftMask(root.lastPiecesMask,
                                                         root.piecesOnCellMask);

        if(piecesLeftMask !== 0)
        {
            for(let i = 0; i < (Helper.PlayerNumber.Player6 - 1); i++)
            {
                if((piecesLeftMask & (1 << i)) !== 0)
                {
                    console.log("[" + (root.cellNumber).toString()
                                + ", "+ (root.orderIndex).toString()
                                + "]: Removing gaming piece of Player" + (i+1).toString()
                                + " (" + _playersInfoModel.getPlayerNicknameByNumber(i+1) + ")");
                    _gamingPiecesDisplay.removePiece(i+1);
                }
            }
        }

        let piecesAppearMask = Helper.definePiecesAppearMask(root.lastPiecesMask,
                                                             root.piecesOnCellMask);
        if(piecesAppearMask !== 0)
        {
            for(let j = 0; j < (Helper.PlayerNumber.Player6 - 1); j++)
            {
                if((piecesAppearMask & (1 << j)) !== 0)
                {
                    console.log("[" + (root.cellNumber).toString()
                                + ", "+ (root.orderIndex).toString()
                                + "]: Adding gaming piece of Player" + (j+1).toString()
                                + " (" + _playersInfoModel.getPlayerNicknameByNumber(j+1) + ")");
                    _gamingPiecesDisplay.addPiece(j+1);
                }
            }
        }
    }
}
