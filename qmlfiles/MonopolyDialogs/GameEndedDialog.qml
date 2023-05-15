import QtQuick 2.15

MonopolyDialog
{
    id: root

    dialogWindowTitle: "Апчихба"

    Text
    {
        id: _gameEndedLabel

        anchors.top: root.headerId.bottom
        anchors.horizontalCenter: root.headerId.horizontalCenter

        text: "Игра окончена"
    }
}
