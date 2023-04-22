import QtQuick 2.15
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import "MonopolyComponents"
import "MonopolyComponents/SubComponents"
import "HelperSingletone"

Window
{
    property int sizeUnit: (_win.height + _win.width)*0.1
    property double defaultMargin: sizeUnit*0.15
    property double componentsBorderCoeff: 0.015
    property double componentsRadiusCoeff: 0.035

    // Delete later!
    property int debugCellsCount: 0
    property int debugPieceIter: Helper.PlayerNumber.Player1
    // -------------


    id: _win
    minimumWidth: 800
    minimumHeight: 600
    width: minimumWidth
    height: minimumHeight
    visible: true
    title: qsTr("Игра \"Монополия\"")

    visibility: _toggleVisibilityButton.isWinFullScreen ? "FullScreen" : "Maximized"

    Rectangle
    {
        property color shareGradColor1: Helper.makeRgb(0, 0, 0)
        property color shareGradColor2: Helper.makeRgb(0, 0, 0)
        property color backgroundColor1: Qt.lighter(Helper.makeRgb(90, 202, 255), 1.3)
        property color backgroundColor2: Qt.lighter(Helper.makeRgb(155, 255, 199), 1.1)
        readonly property int backgroundAnimationDuration: 10000

        id: _displayField
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
            height: sizeUnit*0.32
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
            imageSource:  isWinFullScreen ? "../../assets/svgs/misc/window_mode.svg" :
                                            "../../assets/svgs/misc/full_screen.svg"
            opacityMinBorder: 0.5
            onClicked:
            {
                isWinFullScreen = !isWinFullScreen;
            }
        }

        CurrentStageInfoBlock
        {

        }

        WhosTurnInfoBlock
        {

        }

        PlayersInfoBlock
        {

        }

        EndConditionsInfoBlock
        {
            id: _endConditionsBlock
            sharedColor: _displayField.shareGradColor1
            state: "folded"
            height: state === "unfolded" ? sizeUnit*0.35 : sizeUnit*0.25
            width: state === "unfolded" ? height*3.3 : height
            anchors.top: _toggleVisibilityButton.bottom
            anchors.left: _displayField.left
            anchors.leftMargin: defaultMargin
            anchors.topMargin: defaultMargin
        }

        Keys.onPressed: (event) =>
        {
            switch(event.key)
            {
            case Qt.Key_F11:
                _toggleVisibilityButton.isWinFullScreen = !_toggleVisibilityButton.isWinFullScreen;
                event.accepted = true;
                break;
            case Qt.Key_F10:
                _map.refresh()
                event.accepted = true;
                break;
            case Qt.Key_F9:
                _win.debugCellsCount++;
                _map.addField();
                break;
            case Qt.Key_F8:
                if(_win.debugCellsCount != 0)
                {
                    _win.debugCellsCount--;
                    _map.delField();
                }
                break;
            case Qt.Key_F7:
                _win.debugCellsCount = 9;
                _map.resetField();
                break;
            case Qt.Key_F6:
                _map.placeGamingPiece(0, debugPieceIter);
                if(debugPieceIter !== Helper.PlayerNumber.Player6 + 1)
                    debugPieceIter++;
                break;
            case Qt.Key_F5:
                _map.delLastPiece(0);
                if(debugPieceIter !== Helper.PlayerNumber.Player1)
                    debugPieceIter--;
                break;
            }
        }

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
                                       _displayField.shareGradColor1 = _firstGrad.color
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
}
