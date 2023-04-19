import QtQuick 2.15
import QtQuick.Controls

Window
{
    property int sizeUnit: (_win.height + _win.width)*0.1
    property double defaultMargin: sizeUnit*0.15
    property int debugCellsCount: 0

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
        id: _displayField
        anchors.fill: parent
        color: Helpers.makeRgb(224, 240, 255);

        FieldsGrid
        {
            id: _map
            anchors.horizontalCenter: _displayField.horizontalCenter
            anchors.bottom: _diceBlock.bottom
            height: sizeUnit*mapScale
            width: height
            border.width: 2
            border.color: Qt.rgba(_displayField.color.r-0.25,
                                  _displayField.color.g-0.25,
                                  _displayField.color.b-0.25)
        }

        MonopolyButton
        {
            id: _activateFieldButton
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
            height: _endTurnButton.height*2
            width: _endTurnButton.width
            anchors.right: _displayField.right
            anchors.bottom: _displayField.bottom
            anchors.margins: defaultMargin
        }

        MonopolyButton
        {
            id: _giveUpButton
            state: "normal"
            height: sizeUnit*0.20
            width: height*2.5
            anchors.bottom: _displayField.bottom
            anchors.left: _displayField.left
            anchors.margins: defaultMargin
            textContent: "Сдаться"
        }

        MonopolyButton
        {
            id: _toggleVisibilityButton
            state: "normal"
            property bool isWinFullScreen: false
            height: sizeUnit*0.15
            width: height
            anchors.left: _displayField.left
            anchors.top: _displayField.top
            anchors.margins: defaultMargin
            imageSource:  isWinFullScreen ? "../assets/svgs/misc/window_mode.svg" :
                                            "../assets/svgs/misc/full_screen.svg"
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
            }
        }
    }
}
