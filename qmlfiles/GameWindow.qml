import QtQuick 2.15
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import "MonopolyComponents"
import "MonopolyComponents/InfoBlocks"
import "MonopolyComponents/InfoBlocks/FoldingInfoBlocks"
import "MonopolyComponents/SubComponents"
import "MonopolyDialogs"
import "HelperSingletone"
import GameMap 1.0

Window
{
    id: _win

    readonly property int sizeUnit: (_win.height + _win.width)*0.1
    readonly property double defaultMargin: sizeUnit*0.12
    readonly property double componentsBorderCoeff: 0.015
    readonly property double componentsRadiusCoeff: 0.035
    readonly property int tooltipDelayMs: 700
    readonly property int stageAnimationDurationMs: 5100

    property int hostPlayerNumber: Helper.PlayerNumber.Player2
    property bool isTurnNotiEnabled: true
    property int turnTime: 10
    property int currentStage: 1

    // Delete later!
    property int debugCellsCount: 0
    property int debugPieceIter: Helper.PlayerNumber.Player1
    property int whatDice: Helper.Dice.Left
    // -------------

    minimumWidth: 800
    minimumHeight: 600
    visible: true
    title: qsTr("Игра \"Монополия\"")

    visibility: _toggleVisibilityButton.isWinFullScreen ? "FullScreen" : "Maximized"

    Rectangle
    {
        id: _displayField

        property color backgroundColor1: Qt.lighter(Helper.makeRgb(90, 202, 255), 1.3)
        property color backgroundColor2: Qt.lighter(Helper.makeRgb(155, 255, 199), 1.1)
        property color shareGradColor1
        property color shareGradColor2
        readonly property int backgroundAnimationDuration: 10000

        anchors.fill: parent
        color: "transparent"
        border.width: sizeUnit/40
        border.color: shareGradColor2

        FieldsGrid
        {
            id: _map
            anchors.horizontalCenter: _displayField.horizontalCenter
            anchors.bottom: _diceBlock.bottom
            anchors.top: _toggleVisibilityButton.bottom
            height: sizeUnit*mapScale
            width: height
            sharedColor: _displayField.shareGradColor1
        }

        MonopolyButton
        {
            id: _activateFieldButton
            sharedColor: _displayField.shareGradColor2
            state: "normal"
            height: _endTurnButton.height
            width: _endTurnButton.width
            anchors.horizontalCenter: _endTurnButton.horizontalCenter
            anchors.bottom: _endTurnButton.top
            anchors.bottomMargin: defaultMargin
            textContent: "Применить действие"
        }

        MonopolyButton
        {
            id: _endTurnButton
            sharedColor: _displayField.shareGradColor2
            state: "normal"
            height: sizeUnit*0.36
            width: height*3
            anchors.bottom: _diceBlock.top
            anchors.horizontalCenter: _diceBlock.horizontalCenter
            anchors.bottomMargin: defaultMargin
            textContent: "Закончить ход"
        }

        DiceBlock
        {
            id: _diceBlock
            sharedColor: _displayField.shareGradColor2
            leftDiceColor: _displayField.shareGradColor1
            rightDiceColor: _displayField.shareGradColor2
            height: _endTurnButton.height*2
            width: _endTurnButton.width
            anchors.right: _displayField.right
            anchors.bottom: _displayField.bottom
            anchors.margins: defaultMargin

            Component.onCompleted: { _diceBlock.diceStop(); }
        }

        MonopolyButton
        {
            id: _giveUpButton
            sharedColor: _displayField.shareGradColor1
            state: "normal"
            height: sizeUnit*0.25
            width: height*2.5
            anchors.bottom: _displayField.bottom
            anchors.left: _displayField.left
            anchors.margins: defaultMargin
            textContent: "Сдаться"
            opacityMinBorder: 0.5
        }

        MonopolyButton
        {
            id: _toggleVisibilityButton
            sharedColor: _displayField.shareGradColor1
            state: "normal"
            property bool isWinFullScreen: false
            height: sizeUnit*0.25
            width: height
            anchors.left: _displayField.left
            anchors.top: _displayField.top
            anchors.margins: defaultMargin
            imageSource:  isWinFullScreen ? "../../assets/svgs/misc/full_screen.svg" :
                                            "../../assets/svgs/misc/window_mode.svg"
            opacityMinBorder: 0.5
            tooltipText: isWinFullScreen ? "Перейти в оконный режим"
                                         : "Перейти в полноэкранный режим"
            onClicked:
            {
                isWinFullScreen = !isWinFullScreen;
            }
        }

        MonopolyButton
        {
            id: _toggleTurnNotification
            sharedColor: _displayField.shareGradColor1
            state: "normal"
            height: sizeUnit*0.25
            width: height
            anchors.left: _toggleVisibilityButton.right
            anchors.top: _displayField.top
            anchors.margins: defaultMargin
            imageSource:  isTurnNotiEnabled ? "../../assets/svgs/misc/notification_bell_enabled.svg"
                                            : "../../assets/svgs/misc/notification_bell_disabled.svg"
            imageSizeCoeff: 0.6
            opacityMinBorder: 0.5
            tooltipText: isTurnNotiEnabled ? "Выключить уведомление о ходе"
                                           : "Включить уведомление о ходе"
            onClicked:
            {
                isTurnNotiEnabled = !isTurnNotiEnabled;
            }
        }

        CurrentStageInfoBlock
        {
            id: _currentStageInfoBlock
            anchors.horizontalCenter: _displayField.horizontalCenter
            anchors.top: _toggleVisibilityButton.top
            width: sizeUnit*2
            height: sizeUnit/5
        }

        PlayersInfoBlock
        {
            id: _playersInfoBlock
            anchors.top: _displayField.top
            anchors.right: _displayField.right
            anchors.margins: defaultMargin
            width: _endTurnButton.width
            sharedColor: _displayField.shareGradColor2
        }

        EndConditionsInfoBlock
        {
            id: _endConditionsBlock
            sharedColor: _displayField.shareGradColor1
            state: "unfolded"
            unfoldedHeight: sizeUnit*0.35
            unfoldedWidth: unfoldedHeight*3.3
            foldedHeight: sizeUnit*0.25
            foldedWidth: foldedHeight
            anchors.top: _toggleVisibilityButton.bottom
            anchors.left: _displayField.left
            anchors.leftMargin: defaultMargin
            anchors.topMargin: defaultMargin
        }

        WhosTurnInfoBlock
        {
            id: _whosTurnInfoBlock
            sharedColor: _displayField.shareGradColor1
            state: "unfolded"
            unfoldedHeight: _endConditionsBlock.unfoldedHeight
            unfoldedWidth: _endConditionsBlock.unfoldedWidth
            foldedHeight: _endConditionsBlock.foldedHeight
            foldedWidth: _endConditionsBlock.foldedWidth
            anchors.left: _displayField.left
            anchors.top: _endConditionsBlock.bottom
            anchors.margins: defaultMargin
            turnTime: _win.turnTime
        }

        HostInfoBlock
        {
            id: _hostInfoBlock
            sharedColor: _displayField.shareGradColor1
            state: "unfolded"
            unfoldedWidth: _whosTurnInfoBlock.unfoldedWidth
            foldedHeight: _whosTurnInfoBlock.foldedHeight
            foldedWidth: _whosTurnInfoBlock.foldedWidth
            anchors.left: _displayField.left
            anchors.top: _whosTurnInfoBlock.bottom
            anchors.margins: defaultMargin
        }

        CellDialog
        {
            id: _cellDialog
            visible: false
            height: _win.sizeUnit*1.5
            width: height*1.5
            x: _displayField.width/2 - width/2
            y: _displayField.height/2 - height/2
        }

        function showCellDialog(inputFieldType: int, inputOwnerPlayerNumber: int, inputFieldCost: int,
                                inputFieldIncome: int, inputArrowDirection: int)
        {
            if(!Helper.canFieldExecuteDialog(inputFieldType))
                return;

            _cellDialog.x = _displayField.width/2 - _cellDialog.width/2;
            _cellDialog.y = _displayField.height/2 - _cellDialog.height/2;

            _cellDialog.showFieldType = inputFieldType;
            _cellDialog.showOwnerPlayerNumber = inputOwnerPlayerNumber;
            _cellDialog.showFieldCost = inputFieldCost;
            _cellDialog.showFieldIncome = inputFieldIncome;
            _cellDialog.showArrowDirection = inputArrowDirection;

            _cellDialog.visible = true;
        }

        // Models
        FieldsGridModel
        {
            id: _fieldsGridModel
            list: _cellsList
        }

        // Hotkeys. Most of these keys are debug only and they will be deleted soon!
        Keys.onPressed: (event) =>
        {
            switch(event.key)
            {
            case Qt.Key_F11:
                _toggleVisibilityButton.isWinFullScreen = !_toggleVisibilityButton.isWinFullScreen;
                break;
            case Qt.Key_Escape:
                if(_cellDialog.visible)
                    _cellDialog.visible = false;
                break;
            case Qt.Key_F4:
                _playersInfoBlock.addPlayerRow();
                break;
            case Qt.Key_F3:
                _playersInfoBlock.delPlayerRow();
                break;
            case Qt.Key_F2:
                _playersInfoBlock.debugChangePlayerBalance();
                break;
            case Qt.Key_0:
                _diceBlock.diceStop();
                break;
            case Qt.Key_9:
                _diceBlock.diceGoInfiniteRotation();
                break;
            case Qt.Key_8:
                whatDice = (whatDice === Helper.Dice.Left) ? Helper.Dice.Right : Helper.Dice.Left;
                break;
            case Qt.Key_7:
                _diceBlock.diceRotateToNumbers(Helper.getRandomInt(1,6),
                                               Helper.getRandomInt(1,6));
                break;
            case Qt.Key_BracketLeft:
                _hostInfoBlock.debugRemoveCoalStation();
                break;
            case Qt.Key_BracketRight:
                _hostInfoBlock.debugAddCoalStation();
                break;
            default:
                return;
            }
            if((event.key >= Qt.Key_1) && (event.key <= Qt.Key_6))
            {
                _diceBlock.doDirectRotateTo(whatDice, (event.key - 48));
            }
            event.accepted = true;
        }

        // Background and Share-gradient-color animations
        LinearGradient
        {
            id: _linearGradient
            anchors.fill: _displayField
            start: Qt.point(_displayField.width, 0)
            end: Qt.point(0, _displayField.height)
            z: -1
            gradient: Gradient
            {
                GradientStop { id: _firstGrad
                               position: 0.0;
                               color: _displayField.backgroundColor1
                               PropertyAnimation on color
                               {
                                   id: _backgroundAnimation1
                                   from: _displayField.backgroundColor1;
                                   to: _displayField.backgroundColor2;
                                   duration: _displayField.backgroundAnimationDuration
                                   onFinished:
                                   {
                                       let changer;
                                       changer = _backgroundAnimation1.from.toString();
                                       _backgroundAnimation1.from = _backgroundAnimation1.to.toString();
                                       _backgroundAnimation1.to = changer.toString();
                                       _backgroundAnimation1.restart();
                                   }
                               }
                             }
                GradientStop { id: _secondGrad
                               position: 1.0;
                               color: _displayField.backgroundColor2
                               PropertyAnimation on color
                               {
                                   id: _backgroundAnimation2
                                   from: _displayField.backgroundColor2;
                                   to: _displayField.backgroundColor1;
                                   duration: _displayField.backgroundAnimationDuration
                                   onFinished:
                                   {
                                       let changer;
                                       changer = _backgroundAnimation2.from.toString();
                                       _backgroundAnimation2.from = _backgroundAnimation2.to.toString();
                                       _backgroundAnimation2.to = changer.toString();
                                       _backgroundAnimation2.restart();
                                   }
                               }
                             }
            }
        }

        PropertyAnimation on shareGradColor1
        {
            id: _shareGradAnimation1
            from: _displayField.backgroundColor2
            to: _displayField.backgroundColor1
            duration: _displayField.backgroundAnimationDuration
            onFinished:
            {
                let changer;
                changer = _shareGradAnimation1.from.toString();
                _shareGradAnimation1.from = _shareGradAnimation1.to.toString();
                _shareGradAnimation1.to = changer.toString();
                _shareGradAnimation1.restart();
            }
        }
        PropertyAnimation on shareGradColor2
        {
            id: _shareGradAnimation2
            from: _displayField.backgroundColor1
            to: _displayField.backgroundColor2
            duration: _displayField.backgroundAnimationDuration
            onFinished:
            {
                let changer;
                changer = _shareGradAnimation2.from.toString();
                _shareGradAnimation2.from = _shareGradAnimation2.to.toString();
                _shareGradAnimation2.to = changer.toString();
                _shareGradAnimation2.restart();
            }
        } 
    }

    StageDisplay
    {
        id: _stageDisplay
        anchors.fill: parent
        visible: false;
    }

    Connections
    {
        target: _gameTransmitter
        function onSetCurrentGameStage(stageNumber: int, withAnimation: bool)
        {
            _win.currentStage = stageNumber;

            if(!withAnimation)
                return;

            _map.toggleOverlayRectangleOpacity();
            _stageDisplay.startStageAnimation();
        }
    }

    signal qmlGameWindowClosed();

    onVisibilityChanged:
    {
        if(_win.visibility == Window.Hidden)
        {
            qmlGameWindowClosed();
        }
    }
}
