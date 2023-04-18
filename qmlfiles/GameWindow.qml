import QtQuick 2.15
import QtQuick.Controls

Window
{
    property int sizeUnit: (_win.height + _win.width)*0.1
    property double defaultMargin: sizeUnit*0.05

    id: _win
    minimumWidth: 800
    minimumHeight: 600
    width: minimumWidth
    height: minimumHeight
    visible: true
    title: qsTr("Игра \"Монополия\"")

    //visibility: _toggleVisibilityButton.toggler ? "Maximized" : "FullScreen"
    visibility: "Maximized"

    Rectangle
    {
        id: _displayField
        anchors.fill: parent
        color: makeRgb(237, 255, 230);

        function makeRgb(r, g, b, alpha = 1)
        {
            const max = 255;
            return Qt.rgba(r/max, g/max, b/max, alpha);
        }

        FieldsGrid
        {
            id: _map
            anchors.centerIn: _displayField
            anchors.bottomMargin: defaultMargin
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
            anchors.bottomMargin: defaultMargin*2
            textContent: "Применить действие"

        }

        MonopolyButton
        {
            id: _endTurnButton
            height: sizeUnit*0.32
            width: height*3
            anchors.bottom: _diceBlock.top
            anchors.horizontalCenter: _diceBlock.horizontalCenter
            anchors.bottomMargin: defaultMargin*2
            textContent: "Закончить ход"
        }

        DiceBlock
        {
            id: _diceBlock
            height: sizeUnit*0.7
            width: height*1.5
            color: "transparent"
            anchors.right: _displayField.right
            anchors.bottom: _displayField.bottom
            anchors.rightMargin: defaultMargin*5
            anchors.bottomMargin: defaultMargin
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
            property bool toggler: false
            height: sizeUnit*0.25
            width: height
            anchors.left: _displayField.left
            anchors.top: _displayField.top
            anchors.margins: defaultMargin
            imageSource:  toggler ? "../assets/svgs/misc/full_screen.svg" :
                                    "../assets/svgs/misc/window_mode.svg"
            imageColorOverlay: Qt.lighter(_toggleVisibilityButton.color, 1.5)
            onClicked:
            {
                toggler = !toggler;
                imageSource = toggler ? "../assets/svgs/misc/full_screen.svg" :
                                        "../assets/svgs/misc/window_mode.svg";
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
    }

}
