import QtQuick 2.15
import "../HelperSingletone"
import "../MonopolyComponents"

Rectangle
{
    id: root

    property string dialogWindowTitle: "Диалоговое окно"
    property bool canBeDragged: true
    property bool hasCloseButton: true

    property alias headerId: _header
    property alias dialogTitleId: _dialogTitle

    color: Qt.darker(_displayField.shareGradColor1, 1.1)

    radius: _win.componentsRadiusCoeff*900

    property double heightCoeff: 1
    property double widthCoeff: 1

    readonly property double baseHeight: _win.sizeUnit*1.25
    readonly property double baseWidght: _win.sizeUnit*2.25

    height: baseHeight * heightCoeff
    width: baseWidght * widthCoeff

    Rectangle
    {
        id: _header
        anchors.horizontalCenter: root.horizontalCenter
        anchors.top: root.top
        width: root.width
        height: width/15
        color: _mouseAreaDragger.containsPress ? Qt.darker(_displayField.shareGradColor1, 1.45)
                                               : Qt.darker(_displayField.shareGradColor1, 1.35)

        radius: _win.componentsRadiusCoeff*200

        Text
        {
            id: _dialogTitle

            anchors.verticalCenter: _header.verticalCenter
            anchors.left: _header.left
            anchors.leftMargin: _win.defaultMargin/4

            text: root.dialogWindowTitle
            color: Qt.lighter(root.color, 1.7)
            font.family: "Bookman Old Style"
            font.pointSize: Helper.fontWarningPreventor(root.width*0.025)
        }

        MonopolyButton
        {
            id: _closeDialogButton
            textContent: "X"
            width: _header.width/18
            height: width
            anchors.right: _header.right
            anchors.verticalCenter: _header.verticalCenter
            anchors.rightMargin: _win.defaultMargin/6
            sharedColor: root.color
            hasText: true
            tooltipText: "Закрыть"
            state: "normal"
            z: 3

            visible: root.hasCloseButton

            onClicked:
            {
                root.visible = false
            }
        }

        Rectangle
        {
            id: _subHeaderFooter
            width: _header.width
            height: _header.radius
            color: _header.color
            anchors.horizontalCenter: _header.horizontalCenter
            anchors.bottom: _header.bottom
        }

        MouseArea
        {
            id: _mouseAreaDragger
            anchors.fill: _header
            drag.target: root
            visible: root.canBeDragged
            enabled: visible
        }
    }

    MouseArea
    {
        id: _mouseAreaClickSilencer
        anchors.fill: root
        enabled: root.visible
        visible: enabled
        z: -1
    }
}



