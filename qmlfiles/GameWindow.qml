import QtQuick 2.15
import QtQuick.Controls

Window
{
    id: _win
    width: 800
    height: 600
    minimumWidth: 800
    minimumHeight: 600

    property int sizeUnit: (_win.height + _win.width)*0.1

    visible: true
    title: qsTr("Игра \"Монополия\"")

    FieldsGrid
    {
        id: _map
        height: sizeUnit*mapScale
        width: height
        anchors.centerIn: parent
    }
}
