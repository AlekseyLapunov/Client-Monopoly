import QtQuick 2.15
import Qt5Compat.GraphicalEffects

Rectangle
{
    id: root
    property color baseColor: "lightgreen"
    property string imageSource: ""
    property color imageColorOverlay: "lightgreen"
    property string textContent: ""

    property bool hasText: textContent.length > 0
    property bool hasImage: imageSource.length > 0

    color: _mouseArea.containsPress ? Qt.darker(root.baseColor, 1.2) : root.baseColor
    radius: (height + width)*0.04

    signal clicked()

    MouseArea
    {
        id: _mouseArea
        anchors.fill: root
        cursorShape: "PointingHandCursor"
        onClicked:
        {
            root.clicked()
        }
    }

    Rectangle
    {
        id: _backgroundRect
        property double mockingValue: 10
        anchors.centerIn: root
        width: root.width - mockingValue
        height: root.height - mockingValue
        color: Qt.darker(root.color, 1.1)
        radius: root.radius - mockingValue/2

        Text
        {
            id: _text
            anchors.centerIn: _backgroundRect
            text: root.hasText ? root.textContent : ""
            color: Qt.lighter(root.color, 1.5)
            font.family: "Consolas"
            font.pointSize: (1/(Math.sqrt(textContent.length)+1))*root.width*0.4
        }

        Image
        {
            id: _image
            anchors.fill: _backgroundRect
            source: root.hasImage ? root.imageSource : ""
        }

        ColorOverlay
        {
            anchors.fill: _image
            source: _image
            color: imageColorOverlay
        }
    }
}
