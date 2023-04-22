import QtQuick 2.15
import Qt5Compat.GraphicalEffects
import "../HelperSingletone"

Rectangle
{
    id: root
    property color sharedColor: Qt.rgba(0, 0, 0, 0)
    property color baseColor: Qt.rgba(sharedColor.r-0.25,
                                      sharedColor.g-0.25,
                                      sharedColor.b-0.25)
    property string imageSource: ""
    property string textContent: ""

    property bool hasText: textContent.length > 0
    property bool hasImage: imageSource.length > 0

    property double opacityMinBorder: 1.0
    property double opacityMaxBorder: 1.0

    radius: (height + width)*0.035

    border.width: (height+width)*_win.componentsBorderCoeff
    border.color: Qt.lighter(root.color, 1.2)

    opacity: _hoverHandler.hovered ? opacityMaxBorder : opacityMinBorder

    signal clicked()

    MouseArea
    {
        id: _mouseArea
        anchors.fill: root
        cursorShape: "PointingHandCursor"
        preventStealing: true
        onClicked:
        {
            root.clicked()
        }
        onContainsPressChanged:
        {
            _mouseArea.containsPress ? root.state = "underPressure" : root.state = "normal"
        }
        HoverHandler
        {
            id: _hoverHandler
        }
    }

    Text
    {
        id: _text
        anchors.centerIn: root
        text: root.hasText ? root.textContent : ""
        color: Qt.lighter(root.color, 1.7)
        font.family: "Bookman Old Style"
        font.pointSize: (1/(Math.sqrt(textContent.length)*1.3+1))*root.width*0.4
    }

    Image
    {
        id: _image
        //anchors.fill: root
        anchors.centerIn: root
        source: root.hasImage ? root.imageSource : ""
        sourceSize.height: root.height - 6*root.border.width
        sourceSize.width: root.height - 6*root.border.width

        smooth: true
    }

    ColorOverlay
    {
        anchors.fill: _image
        source: _image
        color: Qt.lighter(root.color, 1.7)
    }

    Behavior on opacity { PropertyAnimation { duration: 100 } }

    states:
    [
        State
        {
            name: "normal"
            PropertyChanges
            {
                target: root
                color: root.baseColor
            }
            PropertyChanges
            {
                target: _mouseArea
                enabled: true
            }
        },
        State
        {
            name: "underPressure"
            PropertyChanges
            {
                target: root
                color: Qt.darker(root.baseColor, 1.2)
            }
            PropertyChanges
            {
                target: _mouseArea
                enabled: true
            }
        },
        State
        {
            name: "disabled"
            PropertyChanges
            {
                target: root
                color: Helper.makeRgb(153, 153, 153)
            }
            PropertyChanges
            {
                target: _mouseArea
                enabled: false
            }
        },
        State
        {
            name: "hidden"
            PropertyChanges
            {
                target: root
                visible: false
            }
            PropertyChanges
            {
                target: _mouseArea
                enabled: false
            }
        }
    ]
}
