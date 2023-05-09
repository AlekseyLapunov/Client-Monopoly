import QtQuick 2.15
import QtMultimedia
import "../../../HelperSingletone"

FoldingInfoBlock
{
    id: root

    property int whatPlayerNumber: Helper.PlayerNumber.Player1
    property string playerNickname///: _playersInfoModel.getPlayerNicknameByNumber(whatPlayerNumber)
    property int turnTime: _win.turnTime

    foldedImagePath: "../../../../assets/svgs/misc/turn_info_folded_image";
    imageColorOverlay: (whatPlayerNumber === Helper.PlayerNumber.NoPlayer) ? Qt.lighter(root.color, 1.4)
                                                                           : Helper.definePlayerColorByNumber(whatPlayerNumber);

    tooltipText: (state === "unfolded") ? "Свернуть вкладку"
                                        : "Развернуть информацию о текущем ходе"

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
        font.pointSize: Helper.fontWarningPreventor(root.width*0.048)
        font.underline: true
        font.bold: true
        text: "Текущий ход"
        Behavior on opacity { NumberAnimation { easing.type: Easing.InQuad} }
    }

    property double subLabelsCoeff: 0.042

    Text
    {
        id: _playerTurnLabel
        opacity: labelsOpacity
        anchors.left: root.left
        anchors.top: root.top
        anchors.leftMargin: _win.defaultMargin/1.2
        anchors.topMargin: _win.defaultMargin*1.1
        font.family: "Bookman Old Style"
        font.pointSize: Helper.fontWarningPreventor(root.width*subLabelsCoeff)
        color: _upperLabel.color
        text: "Игрок: "
        Text
        {
            id: _playerTurnSubLabel
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.right
            opacity: labelsOpacity
            font.family: parent.font.family
            font.pointSize: parent.font.pointSize
            text: playerNickname
            color: Helper.definePlayerColorByNumber(whatPlayerNumber)
            style: Text.Outline
            styleColor: Qt.darker(color, 1.4)
        }

        Behavior on opacity { NumberAnimation { easing.type: Easing.InQuad} }
    }

    Text
    {
        id: _timeLeftLabel
        opacity: labelsOpacity
        anchors.top: _playerTurnLabel.bottom
        anchors.horizontalCenter: _playerTurnLabel.horizontalCenter
        font.family: "Bookman Old Style"
        font.pointSize: Helper.fontWarningPreventor(root.width*subLabelsCoeff)
        color: _upperLabel.color
        text: "Время: "
        Text
        {
            id: _timeLeftSubLabel
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.right
            opacity: labelsOpacity
            font.family: parent.font.family
            font.pointSize: parent.font.pointSize
            font.bold: true
            text: _turnTimeLeftTimer.counter.toString()
            color: _turnTimeLeftTimer.counter > 3 ? parent.color : Qt.rgba(255,
                                                                           (parent.color.g-50/255),
                                                                           (parent.color.b-50/255))
        }

        Behavior on opacity { NumberAnimation { easing.type: Easing.InQuad} }
    }

    Timer
    {
        id: _turnTimeLeftTimer
        interval: 1000
        repeat: true
        running: true
        property int counter: turnTime
        onTriggered:
        {
            if(counter >= 0)
                counter--;
            // !!! Debug !!!
            if(counter === -1)
            {
                counter = turnTime;
                root.whatPlayerNumber = (root.whatPlayerNumber < _playersInfoModel.rowCount()) ?
                                        (root.whatPlayerNumber + 1) :
                                        (Helper.PlayerNumber.Player1);
                updateWhosTurnData();
                if(root.whatPlayerNumber === _win.hostPlayerNumber && _win.isTurnNotiEnabled)
                    _turnReminder.play();
            }
        }
    }

    SoundEffect
    {
        id: _turnReminder
        source: "../../../../assets/sounds/turn_reminder/turn_reminder.wav"
        volume: 0.15
    }

    Component.onCompleted:
    {
        updateWhosTurnData();
    }

    function updateWhosTurnData()
    {
        playerNickname = _playersInfoModel.getPlayerNicknameByNumber(root.whatPlayerNumber);
    }
}
