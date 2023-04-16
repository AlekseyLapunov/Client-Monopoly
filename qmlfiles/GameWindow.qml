import QtQuick 2.15
import QtQuick.Controls

Window
{
    property int sizeUnit: (_win.height + _win.width)*0.1

    id: _win
    width: 800
    height: 600
    minimumWidth: 800
    minimumHeight: 600

    visible: true
    title: qsTr("Игра \"Монополия\"")

    Rectangle
    {
        id: _displayField
        anchors.fill: parent
        color: Qt.rgba(237/255, 255/255, 230/255, 1);

        FieldsGrid
        {
            id: _map
            anchors.centerIn: _displayField
            height: sizeUnit*mapScale
            width: height
        }

        DiceBlock
        {
            height: sizeUnit*0.7
            width: height*1.5
            color: "transparent"
            anchors.right: _displayField.right
            anchors.bottom: _displayField.bottom
            anchors.rightMargin: sizeUnit*0.2
            anchors.bottomMargin: sizeUnit*0.1
        }
    }

    MonopolyButton
    {
        id: _activateFieldButton

    }

    MonopolyButton
    {
        id: _endTurnButton

    }

    MonopolyButton
    {
        id: _giveUpButton

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
