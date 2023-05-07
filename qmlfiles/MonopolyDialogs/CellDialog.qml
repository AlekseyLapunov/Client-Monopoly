import QtQuick 2.15
import "../HelperSingletone"
import "../MonopolyComponents"
import "../MonopolyComponents/SubComponents"

Rectangle
{
    id: root

    property int showFieldType: Helper.FieldType.Void
    property int showOwnerPlayerNumber: Helper.PlayerNumber.NoPlayer
    property string showOwnerPlayerNickname: "Player0"
    property int showFieldCost: 0
    property int showFieldIncome: 0
    property int showArrowDirection: 0

    property string fieldNameByType: Helper.defineFieldNameByType(root.showFieldType)
    property string fieldDescriptionByType: Helper.defineFieldDescriptionByType(root.showFieldType)

    property bool hasBuyButton: false

    color: Qt.darker(_displayField.shareGradColor1, 1.1)

    radius: _win.componentsRadiusCoeff*900

    readonly property int baseHeight: _win.sizeUnit*1.45

    height: baseHeight - _win.sizeUnit*0.2 + _labelsModel.count*_fieldAdditionalInformation.rowHeight
    width: _win.sizeUnit*2.25

    Cell
    {
        id: _cellDisplay
        height: root.baseHeight*0.45
        width: height

        ignoreClickForDialog: true

        anchors.top: _header.bottom
        anchors.topMargin: _win.defaultMargin/2
        anchors.left: root.left
        anchors.leftMargin: _win.defaultMargin/4

        fieldType: showFieldType
        displayableFieldType: showFieldType
        arrowDirection: showArrowDirection
        playerNumberOwner: showOwnerPlayerNumber

        border.width: 1
        border.color: Qt.lighter(parent.color, 1.2)
    }

    Column
    {
        id: _fieldAdditionalInformation
        property int rowHeight: root.baseHeight/10
        anchors.top: _cellDisplay.bottom
        anchors.topMargin: _win.defaultMargin/2
        anchors.left: root.left
        anchors.leftMargin: _win.defaultMargin/4
        anchors.right: root.right
        anchors.rightMargin: _win.defaultMargin/4
        spacing: _win.defaultMargin/20
        Repeater
        {
            model: _labelsModel
            delegate: Rectangle
            {
                id: _labelsBackground

                width: _fieldAdditionalInformation.width
                height: _fieldAdditionalInformation.rowHeight

                color: Qt.darker(_displayField.shareGradColor1, 1.25)

                Text
                {
                    id: _labelName

                    anchors.left: _labelsBackground.left
                    anchors.leftMargin: _win.defaultMargin/4
                    anchors.verticalCenter: _labelsBackground.verticalCenter

                    text: model.labelName

                    color: Qt.lighter(Helper.defineFieldColorByType(showFieldType), 1.45)
                    style: Text.Outline
                    font.family: "Bookman Old Style"
                    fontSizeMode: Text.Fit
                    font.pixelSize: Math.min(_fieldDescriptionBackground.width, _fieldDescriptionBackground.height)/5

                    Text
                    {
                        id: _labelValue

                        text: model.labelValue
                        anchors.left: parent.right
                        anchors.leftMargin: _win.defaultMargin/10

                        color: model.valueIsColoredDifferent ? Helper.definePlayerColorByNumber(showOwnerPlayerNumber)
                                                             : parent.color
                        style: parent.style
                        font.family: parent.font.family
                        fontSizeMode: parent.fontSizeMode
                        font.pixelSize: parent.font.pixelSize
                    }
                }
            }
        }
    }

    Rectangle
    {
        id: _fieldNameBackground
        height: root.baseHeight/7

        anchors.top: _cellDisplay.top
        anchors.left: _cellDisplay.right
        anchors.leftMargin: _win.defaultMargin/2
        anchors.right: root.right
        anchors.rightMargin: _win.defaultMargin/4
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

            color: Qt.lighter(Helper.defineFieldColorByType(showFieldType), 1.45)
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
            anchors.fill: _header
            drag.target: root
        }
    }

    MonopolyButton
    {
        id: _buyButton
        visible: root.hasBuyButton
        enabled: visible
        textContent: root.showOwnerPlayerNumber === Helper.PlayerNumber.NoPlayer ? "Купить" : "Перекупить"
        width: root.width/3
        height: root.baseHeight/8
        anchors.horizontalCenter: root.horizontalCenter
        anchors.bottom: root.bottom
        anchors.bottomMargin: _win.defaultMargin/2
        sharedColor: root.color
        hasText: true
        state: "normal"
        z: 3

        onClicked:
        {
            // emit signal about buy or overbuy, then close
            root.visible = false
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

    ListModel
    {
        id: _labelsModel
    }

    function fillLabelsModel()
    {
        _labelsModel.clear();
        if(root.showOwnerPlayerNumber !== Helper.PlayerNumber.NoPlayer)
        {
            let item = {};
            item.labelName = "Владелец: ";
            item.labelValue = root.showOwnerPlayerNickname;
            item.valueIsColoredDifferent = true;
            _labelsModel.append(item);
        }
        if(root.showFieldCost > 0)
        {
            let item = {};
            item.labelName = "Стоимость поля: ";
            item.labelValue = (root.showFieldCost.toString() + " млн. ЭК");
            item.valueIsColoredDifferent = false;
            _labelsModel.append(item);
        }
        if(root.showFieldIncome > 0)
        {
            let item = {};
            item.labelName = "Доходность поля: ";
            item.labelValue = (root.showFieldIncome.toString() + " млн. ЭК за ход");
            item.valueIsColoredDifferent = false;
            _labelsModel.append(item);
        }
    }
}



