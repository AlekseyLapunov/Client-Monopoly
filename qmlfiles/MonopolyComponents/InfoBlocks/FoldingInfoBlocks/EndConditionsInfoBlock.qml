import QtQuick 2.15
import Qt5Compat.GraphicalEffects
import "../../../HelperSingletone"

FoldingInfoBlock
{
    id: root

    property int moneyEndCondition: -1
    property int turnsEndCondition: -1

    foldedImagePath: "../../../../assets/svgs/misc/game_conditions_folded.svg";

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
}
