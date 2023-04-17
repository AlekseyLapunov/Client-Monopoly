import QtQuick 2.15

Rectangle
{
    id: root

    property bool isVisible: true
    property string backgroundColor: "transparent"

    border.color: "black"
    border.width: 2;
    visible: isVisible
    color: backgroundColor

    Image
    {
        id: _ownerFrame
    }

    Image
    {
        id: _fieldType
    }

    GamingPiecesDisplay
    {
        id: _gamingPiecesDisplay
    }
}
