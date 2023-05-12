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
import GamePlayers 1.0

Window
{
    id: _win

    readonly property int sizeUnit: (_win.height + _win.width)*0.1
    readonly property double defaultMargin: sizeUnit*0.12
    readonly property double componentsBorderCoeff: 0.015
    readonly property double componentsRadiusCoeff: 0.035
    readonly property int tooltipDelayMs: 700
    readonly property int stageAnimationDurationMs: 5100

    // Set-context-properties
    property int hostPlayerNumber//: Helper.PlayerNumber.Player2
    property bool isTurnNotiEnabled: true
    property int turnTime: 10
    property int currentStage: 1
    property bool using3dDice: true

    // Delete later!
    property int debugCellsCount: 0
    property int debugPieceIter: Helper.PlayerNumber.Player1
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
            x: _displayField.width/2 - width/2
            y: _displayField.height/2 - height/2
        }

        function showCellDialog(inputFieldType: int, inputOwnerPlayerNumber: int,
                                inputOwnerPlayerNickname, inputFieldCost: int,
                                inputFieldIncome: int, inputArrowDirection: int)
        {
            if(!Helper.canFieldExecuteDialog(inputFieldType))
                return;

            _cellDialog.x = _displayField.width/2 - _cellDialog.width/2;
            _cellDialog.y = _displayField.height/2 - _cellDialog.height/2;

            _cellDialog.showFieldType = inputFieldType;
            _cellDialog.showOwnerPlayerNumber = inputOwnerPlayerNumber;
            _cellDialog.showOwnerPlayerNickname = inputOwnerPlayerNickname;
            _cellDialog.showFieldCost = inputFieldCost;
            _cellDialog.showFieldIncome = inputFieldIncome;
            _cellDialog.showArrowDirection = inputArrowDirection;

            _cellDialog.fillLabelsModel();

            _cellDialog.visible = true;
        }

        // Models
        FieldsGridModel
        {
            id: _fieldsGridModel
            list: _cellsList
        }
        PlayersInfoModel
        {
            id: _playersInfoModel
            list: _playersList

            function getPlayerNicknameByNumber(playerNumber: int)
            {
                for(let i = 0; i < _playersInfoModel.rowCount(); i++)
                {
                    if(playerNumber === _playersInfoModel.data((_playersInfoModel.index(i, 0)),
                                                               Helper.PlayerModelRole.PlayerNumberRole))
                    {
                        return _playersInfoModel.data(_playersInfoModel.index(i, 0),
                                                      Helper.PlayerModelRole.DisplayableNameRole);
                    }
                }
                return "";
            }

            function getPlayerBalanceByNumber(playerNumber: int)
            {
                for(let i = 0; i < _playersInfoModel.rowCount(); i++)
                {
                    if(playerNumber === _playersInfoModel.data((_playersInfoModel.index(i, 0)),
                                                               Helper.PlayerModelRole.PlayerNumberRole))
                    {
                        return _playersInfoModel.data(_playersInfoModel.index(i, 0),
                                                      Helper.PlayerModelRole.CurrentBalanceRole);
                    }
                }
                return 0;
            }

            function setPlayerNicknameByNumber(playerNumber, newNickname)
            {
                for(let i = 0; i < _playersInfoModel.rowCount(); i++)
                {
                    if(playerNumber === _playersInfoModel.data((_playersInfoModel.index(i, 0)),
                                                               Helper.PlayerModelRole.PlayerNumberRole))
                    {
                        _playersInfoModel.setData((_playersInfoModel.index(i, 0)),
                                                  newNickname,
                                                  Helper.PlayerModelRole.DisplayableNameRole);
                        break;
                    }
                }
            }

            function setPlayerBalanceByNumber(playerNumber, playerBalance)
            {
                for(let i = 0; i < _playersInfoModel.rowCount(); i++)
                {
                    if(playerNumber === _playersInfoModel.data((_playersInfoModel.index(i, 0)),
                                                               Helper.PlayerModelRole.PlayerNumberRole))
                    {
                        _playersInfoModel.setData((_playersInfoModel.index(i, 0)),
                                                  playerBalance,
                                                  Helper.PlayerModelRole.CurrentBalanceRole);
                        break;
                    }
                }
            }

            function setPlayerPiecePositionByNumber(playerNumber, playerPiecePosition)
            {
                for(let i = 0; i < _playersInfoModel.rowCount(); i++)
                {
                    if(playerNumber === _playersInfoModel.data((_playersInfoModel.index(i, 0)),
                                                               Helper.PlayerModelRole.PlayerNumberRole))
                    {
                        _playersInfoModel.setData((_playersInfoModel.index(i, 0)),
                                                  playerPiecePosition,
                                                  Helper.PlayerModelRole.PiecePositionOnOrderIndexRole);
                        break;
                    }
                }
            }
        }

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
            default:
                event.accepted = true;
                return;
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

        function onSetHostPlayerNumber(newHostPlayerNumber)
        {
            _win.hostPlayerNumber = newHostPlayerNumber;
            _hostInfoBlock.updateInfo();
        }

        function onPlayerNicknameUpdate(inputPlayerNumber,
                                        newPlayerNickname)
        {
            _playersInfoModel.setPlayerNicknameByNumber(inputPlayerNumber, newPlayerNickname);
            _playersInfoBlock.updatePlayersSortableNickname(inputPlayerNumber, newPlayerNickname);
            _hostInfoBlock.updateInfo();
            _whosTurnInfoBlock.updateWhosTurnData();
        }

        function onPlayerBalanceUpdate(inputPlayerNumber,
                                       newBalance)
        {
            _playersInfoModel.setPlayerBalanceByNumber(inputPlayerNumber, newBalance);
            _playersInfoBlock.updatePlayersSortableBalance(inputPlayerNumber, newBalance);
            _hostInfoBlock.updateInfo();
        }

        function onPlayerPiecePositionUpdate(inputPlayerNumber,
                                             newPiecePosition)
        {
            _playersInfoModel.setPlayerPiecePositionByNumber(inputPlayerNumber, newPiecePosition);
        }

        function onSetCurrentGameStage(stageNumber: int, withAnimation: bool)
        {
            _win.currentStage = stageNumber;

            if(!withAnimation)
            {
                _map.setOverlayRectangleOpacityZero();
                return;
            }

            _map.toggleOverlayRectangleOpacity();
            if(_cellDialog.visible)
                _cellDialog.visible = false;
            _stageDisplay.startStageAnimation();
        }

        function onStopDiceAnimation()
        {
            _diceBlock.diceStop();
        }

        function onStartDiceInfiniteAnimation()
        {
            _diceBlock.diceGoInfiniteRotation();
        }

        function onStartDiceDirectionalAnimation(leftDiceValue: int,
                                                 rightDiceValue: int)
        {
            _diceBlock.diceRotateToNumbers(leftDiceValue,
                                           rightDiceValue);
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
