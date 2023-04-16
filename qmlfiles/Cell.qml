import QtQuick 2.15

Rectangle
{

    property bool isVisible: true
    property string backgroundColor: "transparent"
    id: root
    border.color: "black"
    border.width: 2;
    visible: isVisible
    color: backgroundColor
}
