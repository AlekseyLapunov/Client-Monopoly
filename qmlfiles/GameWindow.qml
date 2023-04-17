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
            //anchors.centerIn: _displayField
            anchors.horizontalCenter: _displayField.horizontalCenter
            anchors.bottom: _displayField.bottom
            anchors.bottomMargin: defaultMargin*10
            height: sizeUnit*mapScale
            width: height
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
            id: _activateFieldButton
            height: _endTurnButton.height
            width: _endTurnButton.width
            anchors.right: _displayField.right
            anchors.bottom: _endTurnButton.top
            anchors.rightMargin: _endTurnButton.anchors.rightMargin
            anchors.bottomMargin: defaultMargin*2
            textContent: "Использовать"

        }

        MonopolyButton
        {
            id: _endTurnButton
            height: sizeUnit*0.3
            width: height*3
            anchors.right: _displayField.right
            anchors.bottom: _diceBlock.top
            anchors.rightMargin: defaultMargin*7
            anchors.bottomMargin: defaultMargin*2
            textContent: "Пропустить ход"

        }

        MonopolyButton
        {
            id: _giveUpButton
            height: sizeUnit*0.25
            width: height*2.5
            anchors.bottom: _displayField.bottom
            anchors.right: _diceBlock.left
            anchors.rightMargin: defaultMargin*2.2
            anchors.bottomMargin: defaultMargin*2.2
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
