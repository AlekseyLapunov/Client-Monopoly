import QtQuick 2.15
import QtQuick.Controls

Window
{
    property int sizeUnit: (_win.height + _win.width)*0.1
    property double defaultMargin: sizeUnit*0.15

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
        color: makeRgb(224, 240, 255);

        function makeRgb(r, g, b, alpha = 1)
        {
            const max = 255;
            return Qt.rgba(r/max, g/max, b/max, alpha);
        }

        FieldsGrid
        {
            id: _map
            anchors.horizontalCenter: _displayField.horizontalCenter
            anchors.bottom: _diceBlock.bottom
            height: sizeUnit*mapScale
            width: height
        }

        MonopolyButton
        {
            id: _activateFieldButton
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
            property bool isWinFullScreen: false
            height: sizeUnit*0.25
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
            if(event.key === Qt.Key_F11)
            {
                _toggleVisibilityButton.isWinFullScreen = !_toggleVisibilityButton.isWinFullScreen;
                event.accepted = true;
            }
        }
    }
}
