import QtQuick 2.15
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import "../../../HelperSingletone"

Rectangle
{
    id: root

    property color sharedColor: Qt.rgba(0, 0, 0, 0)
    property color baseColor: Qt.rgba(sharedColor.r-0.25,
                                      sharedColor.g-0.25,
                                      sharedColor.b-0.25)

    property double opacityMinBorder: 1.0
    property double opacityMaxBorder: 1.0

    property double labelsOpacity: 1.0
    property double imageOpacity: 1.0

    property string tooltipText: (state === "unfolded") ? "Свернуть вкладку"
                                                        : "Развернуть вкладку"

    property int unfoldedHeight: 200
    property int unfoldedWidth: 100
    property int foldedHeight: 100
    property int foldedWidth: 50

    property string foldedImagePath: ""
    property color imageColorOverlay: Qt.lighter(root.color, 1.4)

    height: state === "unfolded" ? unfoldedHeight : foldedHeight
    width: state === "unfolded" ? unfoldedWidth : foldedWidth

    radius: (height/Math.pow(width, 1.2))*30

    color: baseColor

    border.width: (height+width)*_win.componentsBorderCoeff/3
    border.color: Qt.lighter(root.color, 1.2)

    ToolTip
    {
        parent: _mouseArea
        visible: _hoverHandler.hovered
        delay: tooltipDelayMs
        text: root.tooltipText
    }

    MouseArea
    {
        id: _mouseArea
        anchors.fill: root
        cursorShape: "PointingHandCursor"
        onClicked:
        {
            if(root.state === "folded")
                root.state = "unfolded"
            else root.state = "folded"
        }
        HoverHandler
        {
            id: _hoverHandler
        }
    }

    Behavior on width { PropertyAnimation { id: _blockWidthAnimation; easing.type: Easing.OutExpo; duration: 800 } }
    Behavior on height { PropertyAnimation { id: _blockHeightAnimation; easing.type: Easing.OutExpo; duration: 200 } }

    Image
    {
        id: _foldedImage
        anchors.centerIn: root
        opacity: imageOpacity
        sourceSize.width: root.width*0.65
        sourceSize.height: root.height*0.65
        source: ((root.state === "folded") ? foldedImagePath
                                           : "")
        smooth: true
    }

    ColorOverlay
    {
        anchors.fill: _foldedImage
        source: _foldedImage
        color: imageColorOverlay
    }

    states:
    [
        State
        {
            name: "folded"
            PropertyChanges
            {
                target: root
                labelsOpacity: 0.0
                imageOpacity: 1.0
                color: Qt.lighter(baseColor, 1.1)
                border.width: (height+width)*_win.componentsBorderCoeff*2
            }
        },
        State
        {
            name: "unfolded"
            PropertyChanges
            {
                target: root
                labelsOpacity: 1.0
                imageOpacity: 0.0
                color: baseColor
                border.width: (height+width)*_win.componentsBorderCoeff/3
            }
        }
    ]

}
