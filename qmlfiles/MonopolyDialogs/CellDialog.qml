import QtQuick 2.15
import "../HelperSingletone"
import "../MonopolyComponents"
import "../MonopolyComponents/SubComponents"

MonopolyDialog
{
    id: root

    property int showFieldType: Helper.FieldType.Void
    property int showOwnerPlayerNumber: Helper.PlayerNumber.NoPlayer
    property string showOwnerPlayerNickname: ""
    property int showFieldCost: 0
    property int showFieldIncome: 0
    property int showArrowDirection: 0

    property string fieldNameByType: Helper.defineFieldNameByType(root.showFieldType)
    property string fieldDescriptionByType: Helper.defineFieldDescriptionByType(root.showFieldType)

    property string hasBuyButtonState: "disabled"

    property bool hasBuyButton: false

    signal buyClicked();

    height: baseHeight + _labelsModel.count*_fieldAdditionalInformation.rowHeight

    Cell
    {
        id: _cellDisplay
        height: root.height*0.45
        width: height

        ignoreClickForDialog: true

        anchors.top: root.headerId.bottom
        anchors.topMargin: _win.defaultMargin/2
        anchors.left: root.left
        anchors.leftMargin: _win.defaultMargin/4

        fieldType: root.showFieldType
        displayableFieldType: root.showFieldType
        arrowDirection: root.showArrowDirection
        playerNumberOwner: root.showOwnerPlayerNumber

        border.width: 1
        border.color: Qt.lighter(parent.color, 1.2)
    }

    Column
    {
        id: _fieldAdditionalInformation
        property int rowHeight: root.baseHeight/10
        anchors.top: _cellDisplay.bottom
        anchors.topMargin: _win.defaultMargin/4
        anchors.left: root.left
        anchors.leftMargin: _win.defaultMargin/4
        anchors.right: root.right
        anchors.rightMargin: _win.defaultMargin/4
        spacing: _win.defaultMargin/4
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
        height: root.height/7

        anchors.top: _cellDisplay.top
        anchors.left: _cellDisplay.right
        anchors.leftMargin: _win.defaultMargin/4
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

    MonopolyButton
    {
        id: _buyButton
        visible: root.hasBuyButton
        enabled: visible
        textContent: root.showOwnerPlayerNumber === Helper.PlayerNumber.NoPlayer ? "Купить"
                                                                                 : "Перекупить"
        width: root.baseWidght/3
        height: root.baseHeight/8
        anchors.horizontalCenter: root.horizontalCenter
        anchors.bottom: root.bottom
        anchors.bottomMargin: _win.defaultMargin/3
        sharedColor: root.color
        hasText: true
        state: hasBuyButtonState
        z: 3

        onClicked:
        {
            root.buyClicked();
        }
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



