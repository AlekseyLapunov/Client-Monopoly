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
    radius: (height + width)*0.035

    border.width: (height+width)*0.02
    border.color: Qt.lighter(color, 1.2)

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

    Text
    {
        id: _text
        anchors.centerIn: root
        text: root.hasText ? root.textContent : ""
        color: Qt.lighter(root.color, 1.5)
        font.family: "Consolas"
        font.pointSize: (1/(Math.sqrt(textContent.length)*1.2+1))*root.width*0.4
    }

    Image
    {
        id: _image
        //anchors.fill: root
        anchors.centerIn: root
        source: root.hasImage ? root.imageSource : ""
        sourceSize.height: root.height - 2*root.border.width
        sourceSize.width: root.height - 2*root.border.width

        smooth: true
    }

    ColorOverlay
    {
        anchors.fill: _image
        source: _image
        color: imageColorOverlay
    }
}
