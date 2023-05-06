import QtQuick 2.15
import "../HelperSingletone"
import "../MonopolyComponents"
import "../MonopolyComponents/SubComponents"

Rectangle
{
    id: root

    property int showFieldType: Helper.FieldType.Void
    property int showOwnerPlayerNumber: Helper.PlayerNumber.NoPlayer
    property int showFieldCost: 0
    property int showFieldIncome: 0

    property string fieldNameByType: Helper.defineFieldNameByType(root.showFieldType)
    property string fieldDescriptionByType: Helper.defineFieldDescriptionByType(root.showFieldType)

    color: Qt.darker(_displayField.shareGradColor1, 1.1)

    radius: _win.componentsRadiusCoeff*900

    Cell
    {
        id: _cellDisplay
        height: root.height*0.45
        width: height

        ignoreClickForDialog: true

        anchors.top: _header.bottom
        anchors.topMargin: _win.defaultMargin/2
        anchors.left: root.left
        anchors.leftMargin: _win.defaultMargin/4

        fieldType: showFieldType
        playerNumberOwner: showOwnerPlayerNumber

        border.width: 1
        border.color: Qt.lighter(parent.color, 1.2)
    }

    Rectangle
    {
        id: _fieldNameBackground
        height: root.height/7

        anchors.top: _cellDisplay.top
        anchors.left: _cellDisplay.right
        anchors.leftMargin: _win.defaultMargin/2
        anchors.right: root.right
        anchors.rightMargin: _win.defaultMargin/2
        color: Qt.darker(_displayField.shareGradColor1, 1.25)

        Text
        {
            id: _fieldName

            text: (Helper.canFieldExecuteDialog(root.showFieldType) ?
                   fieldNameByType :
                   "")

            anchors.verticalCenter: _fieldNameBackground.verticalCenter
            anchors.horizontalCenter: _fieldNameBackground.horizontalCenter

            font.bold: true

            color: Qt.lighter(Helper.defineFieldColorByType(showFieldType), 1.35)
            style: Text.Outline
            font.family: "Bookman Old Style"
            font.pointSize: Helper.fontWarningPreventor((1/(Math.sqrt(fieldNameByType.length)*2.35 + 1))*root.width*0.4)
        }
    }

    Rectangle
    {
        id: _fieldDescriptionBackground
        width: _fieldNameBackground.width

        anchors.top: _fieldNameBackground.bottom
        anchors.topMargin: _win.defaultMargin/4
        anchors.bottom: _cellDisplay.bottom
        anchors.horizontalCenter: _fieldNameBackground.horizontalCenter
        color: Qt.darker(_displayField.shareGradColor1, 1.25)

        Text
        {
            id: _fieldDescription

            text: (Helper.canFieldExecuteDialog(root.showFieldType) ?
                   fieldDescriptionByType :
                   "")

            anchors.fill: _fieldDescriptionBackground
            anchors.margins: _win.defaultMargin/5
            wrapMode: Text.WordWrap

            color: Qt.lighter(Helper.defineFieldColorByType(showFieldType), 1.35)
            style: Text.Outline
            font.family: "Bookman Old Style"
            fontSizeMode: Text.Fit
            font.pixelSize: Math.min(_fieldDescriptionBackground.width, _fieldDescriptionBackground.height)
        }
    }

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

            text: "Информация о поле"
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
            anchors.fill: parent
            drag.target: root
        }
    }
}



