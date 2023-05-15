import QtQuick 2.15
import "../../../HelperSingletone"
import "../../SubComponents"

FoldingInfoBlock
{
    id: root

    property color hostColor: Helper.definePlayerColorByNumber(_win.hostPlayerNumber)
    property string hostNickname
    property int playerNumberBeforeYourTurn
    property color playerColorBeforeYourTurn: Helper.definePlayerColorByNumber(playerNumberBeforeYourTurn)
    property string playerNicknameBeforeYourTurn
    property int energyKrendelsCount
    property int energyKrendelsPerTurn: 0

    property int dataRowHeight: sizeUnit*0.15
    property int dataRowWidth: sizeUnit*1.1

    imageColorOverlay: (hostPlayerNumber === Helper.PlayerNumber.NoPlayer) ? Qt.lighter(root.color, 1.4)
                                                                           : Helper.definePlayerColorByNumber(hostPlayerNumber);

    foldedImagePath: "../../../../assets/svgs/misc/host_info_folded";

    tooltipText: (state === "unfolded") ? "Свернуть вкладку"
                                        : "Развернуть вкладку своей игровой информации"

    unfoldedHeight: (_businessAndResourcesCounted.count === 0) ? sizeUnit*0.5
                    : (sizeUnit*0.60 + _businessAndResourcesCounted.count*(dataRowHeight + _dataRows.spacing)*1.1)

    Rectangle
    {
        id: _overlayRectangleOnFolded
        anchors.centerIn: root
        width: root.width - root.border.width*2
        height: root.height - root.border.width*2
        opacity: (root.state === "folded") ? 0.25 : 0
        color: root.color
        Behavior on opacity
        {
            PropertyAnimation { duration: 500 }
        }
    }

    Text
    {
        id: _upperLabel
        opacity: labelsOpacity
        anchors.horizontalCenter: root.horizontalCenter
        anchors.top: root.top
        anchors.topMargin: _win.defaultMargin/8
        color: Qt.lighter(root.color, 1.7)
        font.family: "Bookman Old Style"
        font.pointSize: Helper.fontWarningPreventor(root.width*0.043)
        font.underline: true
        font.bold: true
        text: "Ваша игровая информация"
        Behavior on opacity { NumberAnimation { easing.type: Easing.InQuad} }
    }

    property double subLabelsCoeff: 0.042

    Text
    {
        id: _playerTurnLabel
        opacity: labelsOpacity
        anchors.top: _upperLabel.bottom
        anchors.horizontalCenter: _upperLabel.horizontalCenter
        font.family: "Bookman Old Style"
        font.pointSize: Helper.fontWarningPreventor(root.width*subLabelsCoeff*1)
        color: _upperLabel.color
        text: "Ваш ход после игрока "

        Behavior on opacity { NumberAnimation { easing.type: Easing.InQuad} }
    }

    Text
    {
        id: _playerBeforeYourTurn
        anchors.topMargin: _win.defaultMargin/8
        anchors.horizontalCenter: _playerTurnLabel.horizontalCenter
        anchors.top: _playerTurnLabel.bottom
        opacity: labelsOpacity
        font.family: _playerTurnLabel.font.family
        font.pointSize: _playerTurnLabel.font.pointSize*0.9
        text: playerNicknameBeforeYourTurn
        color: playerColorBeforeYourTurn
        style: Text.Outline
        styleColor: Qt.darker(color, 1.4)
    }

    Text
    {
        id: _balanceLabel
        anchors.topMargin: _win.defaultMargin/8
        opacity: labelsOpacity
        anchors.top: _playerBeforeYourTurn.bottom
        anchors.left: root.left
        anchors.leftMargin: _win.defaultMargin
        font.family: "Bookman Old Style"
        font.pointSize: Helper.fontWarningPreventor(root.width*subLabelsCoeff*1)
        color: _upperLabel.color
        text: "Баланс: "
        Text
        {
            id: _energyKrendelsCount
            anchors.left: parent.right
            anchors.verticalCenter: parent.verticalCenter
            opacity: labelsOpacity
            font.family: parent.font.family
            font.pointSize: parent.font.pointSize*0.9
            text: root.energyKrendelsCount.toString() + " млн. ЭК"
            color: hostColor
            style: Text.Outline
            styleColor: Qt.darker(color, 1.4)
        }

        Behavior on opacity { NumberAnimation { easing.type: Easing.InQuad} }
    }

    Text
    {
        id: _incomeLabel
        opacity: labelsOpacity
        anchors.top: _balanceLabel.bottom
        anchors.topMargin: _win.defaultMargin/8
        anchors.left: root.left
        anchors.leftMargin: _win.defaultMargin
        font.family: "Bookman Old Style"
        font.pointSize: Helper.fontWarningPreventor(root.width*subLabelsCoeff*1)
        color: _upperLabel.color
        text: "Доход:  "
        Text
        {
            id: _energyKrendelsPerTurn
            anchors.left: parent.right
            anchors.verticalCenter: parent.verticalCenter
            opacity: labelsOpacity
            font.family: parent.font.family
            font.pointSize: parent.font.pointSize*0.9
            font.bold: true
            text: root.energyKrendelsPerTurn.toString() + " млн. ЭК за ход"
            color: parent.color
        }

        Behavior on opacity { NumberAnimation { easing.type: Easing.InQuad} }
    }

    Text
    {
        id: _businessAndResourcesLabel
        visible: !(_businessAndResourcesCounted.count === 0)
        opacity: labelsOpacity
        anchors.horizontalCenter: root.horizontalCenter
        anchors.topMargin: _win.defaultMargin/8
        anchors.top: _incomeLabel.bottom
        color: Qt.lighter(root.color, 1.7)
        font.family: "Bookman Old Style"
        font.pointSize: Helper.fontWarningPreventor(root.width*0.043)
        font.underline: true
        font.bold: true
        text: "Активы и ресурсы"
        Behavior on opacity { NumberAnimation { easing.type: Easing.InQuad} }
    }

    Column
    {
        id: _dataRows
        visible: root.state === "unfolded"
        anchors.top: _businessAndResourcesLabel.bottom
        anchors.horizontalCenter: root.horizontalCenter
        anchors.topMargin: _win.defaultMargin/8
        spacing: sizeUnit/75
        move: Transition {
            NumberAnimation
            {
                easing.type: Easing.OutQuint; properties: "x,y"; duration: 1400
            }
        }

        Repeater
        {
            model: _businessAndResourcesCounted

            delegate: Rectangle
            {
                property int shareFieldType: uniqueFieldType
                property int shareFieldsQuantity: quantity

                height: dataRowHeight*1.2
                width: dataRowWidth
                color: ((index%2 === 0) ? Qt.lighter(root.color, 1.15)
                                        : Qt.darker(root.color, 1.15))

                Cell
                {
                    id: _cell
                    height: parent.height*0.9
                    width: height
                    fieldType: parent.shareFieldType
                    border.width: 1
                    border.color: Qt.lighter(parent.color, 1.2)
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: _win.defaultMargin*0.1
                    ignoreClickForDialog: true
                }

                Text
                {
                    id: _fieldsQuantityLabel
                    anchors.left: _cell.right
                    anchors.leftMargin: _win.defaultMargin/8
                    anchors.verticalCenter: parent.verticalCenter
                    text: ("x" + parent.shareFieldsQuantity.toString())
                    font.bold: true
                    color: _businessAndResourcesLabel.color
                    font.family: "Bookman Old Style"
                    font.pointSize: Helper.fontWarningPreventor(_cell.height*0.50)
                    style: Text.Outline
                    styleColor: (parent.index%2 === 0) ? Qt.darker(parent.color, 1.25)
                                                       : Qt.lighter(parent.color, 1.05)
                }

                Text
                {
                    id: _fieldTypeStringLabel
                    anchors.left: _fieldsQuantityLabel.right
                    anchors.leftMargin: _win.defaultMargin/8
                    anchors.verticalCenter: parent.verticalCenter
                    text: Helper.defineFieldNameByType(shareFieldType)
                    font.bold: false
                    color: _businessAndResourcesLabel.color
                    font.family: "Bookman Old Style"
                    font.pixelSize: text.length < 14 ? parent.width*0.075
                                                     : parent.width*0.05
                    style: Text.Outline
                    styleColor: (parent.index%2 === 0) ? Qt.darker(parent.color, 1.25)
                                                       : Qt.lighter(parent.color, 1.05)
                }
                Behavior on color { PropertyAnimation { duration: 500 } }
            }
        }
    }

    Component.onCompleted:
    {
        updateInfo();
    }

    function updateInfo()
    {
        root.energyKrendelsCount = _playersInfoModel.getPlayerBalanceByNumber(_win.hostPlayerNumber);
        root.playerNumberBeforeYourTurn = Helper.definePlayerNumberBeforeHost(_win.hostPlayerNumber,
                                                                              _playersInfoModel.rowCount());
        root.playerNicknameBeforeYourTurn = _playersInfoModel.getPlayerNicknameByNumber(root.playerNumberBeforeYourTurn);
        root.playerColorBeforeYourTurn = Helper.definePlayerColorByNumber(root.playerNumberBeforeYourTurn);
    }

    Connections
    {
        target: _gameTransmitter

        function onHostIncomeUpdate(newHostIncome)
        {
            root.energyKrendelsPerTurn = newHostIncome;
        }

        function onHostOwningObjectsUpdate(owningFieldType, owningFieldsCount)
        {
            if(owningFieldsCount <= 0)
            {
                for(let i = 0; i < _businessAndResourcesCounted.count; i++)
                {
                    if(owningFieldType === _businessAndResourcesCounted.get(i).uniqueFieldType)
                    {
                        _businessAndResourcesCounted.remove(i);
                        return;
                    }
                }
                return;
            }

            for(let j = 0; j < _businessAndResourcesCounted.count; j++)
            {
                if(owningFieldType === _businessAndResourcesCounted.get(j).uniqueFieldType)
                {
                    _businessAndResourcesCounted.get(j).quantity = owningFieldsCount;
                    return;
                }
            }

            let owningItem = {};
            owningItem.uniqueFieldType = owningFieldType;
            owningItem.quantity = owningFieldsCount;
            _businessAndResourcesCounted.append(owningItem);
        }

        function onHostOwningObjectsClear()
        {
            _businessAndResourcesCounted.clear();
        }
    }

    ListModel
    {
        id: _businessAndResourcesCounted
    }
}
