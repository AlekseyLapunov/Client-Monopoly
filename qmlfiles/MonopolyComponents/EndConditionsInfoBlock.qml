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

    property double opacityMinBorder: 1.0
    property double opacityMaxBorder: 1.0

    property int moneyEndCondition: -1
    property int turnsEndCondition: -1

    property double labelsOpacity: 1.0
    property double imageOpacity: 1.0

    radius: (height/Math.pow(width, 1.2))*30

    color: baseColor

    border.width: (height+width)*_win.componentsBorderCoeff/3
    border.color: Qt.lighter(root.color, 1.2)

    Text
    {
        id: _upperLabel
        opacity: labelsOpacity
        anchors.horizontalCenter: root.horizontalCenter
        anchors.top: root.top
        anchors.topMargin: _win.defaultMargin/8
        color: Qt.lighter(root.color, 1.7)
        font.family: "Bookman Old Style"
        font.pointSize: Helper.fontWarningPreventor(root.width*0.048)
        font.underline: true
        font.bold: true
        text: "Условия окончания игры"

        Behavior on opacity { NumberAnimation { easing.type: Easing.InQuad} }
    }

    property double conditionsLabelsSizeCoeff: 0.042

    Text
    {
        id: _turnsEndConditionLabel
        opacity: labelsOpacity
        anchors.centerIn: root
        font.family: "Bookman Old Style"
        font.pointSize: Helper.fontWarningPreventor(root.width*conditionsLabelsSizeCoeff)
        color: _upperLabel.color
        text: ("Макс. ходов:   " + ((turnsEndCondition !== -1) ? turnsEndCondition.toString() : "Неограниченно"))

        Behavior on opacity { NumberAnimation { easing.type: Easing.InQuad} }
    }
    Text
    {
        id: _moneyEndConditionLabel
        opacity: labelsOpacity
        anchors.top: _turnsEndConditionLabel.bottom
        anchors.horizontalCenter: _turnsEndConditionLabel.horizontalCenter
        font.family: "Bookman Old Style"
        font.pointSize: Helper.fontWarningPreventor(root.width*conditionsLabelsSizeCoeff)
        color: _upperLabel.color
        text: ("Макс. баланс: " + ((moneyEndCondition !== -1) ? (moneyEndCondition.toString() + " млн.") : "Неограниченно"))

        Behavior on opacity { NumberAnimation { easing.type: Easing.InQuad} }
    }

    MouseArea
    {
        id: _mouseArea
        anchors.fill: root
        onClicked:
        {
            if(root.state === "folded")
                root.state = "unfolded"
            else root.state = "folded"
        }
    }

    Behavior on width { PropertyAnimation { id: _blockWidthAnimation; easing.type: Easing.OutExpo; duration: 800 } }
    Behavior on height { PropertyAnimation { id: _blockHeightAnimation; easing.type: Easing.OutExpo; duration: 200 } }

    Image
    {
        id: _foldedImage
        anchors.centerIn: root
        opacity: imageOpacity
        sourceSize.width: root.width*0.75
        sourceSize.height: root.height*0.75
        source: ((root.state === "folded") ? "../../../assets/svgs/misc/game_conditions_folded.svg"
                                           : "")
        smooth: true
    }

    ColorOverlay
    {
        anchors.fill: _foldedImage
        source: _foldedImage
        color: Qt.lighter(root.color, 1.4)
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
