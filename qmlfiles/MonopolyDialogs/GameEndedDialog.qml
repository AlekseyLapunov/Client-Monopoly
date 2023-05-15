import QtQuick 2.15
import "../HelperSingletone"
import "../MonopolyComponents/InfoBlocks"
import "../MonopolyComponents"

MonopolyDialog
{
    id: root

    dialogWindowTitle: "Игра окончена"
    hasCloseButton: false
    canBeDragged: false

    Component.onCompleted:
    {
        dialogTitleId.anchors.centerIn = headerId;
        dialogTitleId.font.bold = true;
    }

    height: baseHeight*0.55 + _topPlayersModel.count*(_playersTopColumn.playerRowHeight*0.8 + _playersTopColumn.spacing)

    Text
    {
        id: _playersTopLabel
        anchors.top: root.headerId.bottom
        anchors.horizontalCenter: root.headerId.horizontalCenter
        anchors.topMargin: _win.defaultMargin/10
        text: "Занятые места"
        font.family: "Bookman Old Style"
        font.pixelSize: root.width/15
        style: Text.Outline
        color: Qt.lighter(root.color, 1.40)
    }

    Column
    {
        id: _playersTopColumn

        property int playerRowHeight: root.baseHeight/5

        anchors.horizontalCenter: _playersTopLabel.horizontalCenter
        anchors.top: _playersTopLabel.bottom
        anchors.topMargin: _win.defaultMargin/5

        spacing: _win.defaultMargin/5

        Repeater
        {
            id: _playersTopRepeater
            model: _topPlayersModel
            delegate: Rectangle
            {
                anchors.horizontalCenter: parent.horizontalCenter
                height: index === 0 ? _playersTopColumn.playerRowHeight
                      : index === 1 ? _playersTopColumn.playerRowHeight*0.9
                      : index === 2 ? _playersTopColumn.playerRowHeight*0.8
                      : _playersTopColumn.playerRowHeight*0.7
                width: height*6
                color: Helper.applyContrast(root.color, (1.4 + (_topPlayersModel.count - index)/15))
                Text
                {
                    id: _playerTopNickname
                    anchors.centerIn: parent
                    text: model.playerName
                    font.pixelSize: text.length < 10 ? parent.height/2 : parent.height/2.5
                    font.family: "Bookman Old Style"
                    font.bold: true
                    style: Text.Outline
                    color: Helper.definePlayerColorByNumber(model.playerNumber)
                }

                Text
                {
                    id: _playerTopPlacement
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: _win.defaultMargin/5
                    text: (index + 1).toString()
                    font.pixelSize: parent.height/2
                    horizontalAlignment: Text.AlignRight
                    font.family: "Bookman Old Style"
                    font.bold: true
                    color: index === 0 ? "goldenrod"
                         : index === 1 ? "silver"
                         : index === 2 ? "chocolate"
                         : "white"
                    style: Text.Outline
                }
            }
        }
    }

    MonopolyButton
    {
        id: _leaveButton
        textContent: "Выйти"
        height: root.baseHeight/10
        width: height*3
        anchors.right: root.right
        anchors.bottom: root.bottom
        anchors.margins: _win.defaultMargin/2
        sharedColor: root.color
        hasText: true
        state: "normal"
        z: 5

        onClicked:
        {
            _win.visibility = Window.Hidden
        }
    }

    Text
    {
        id: _automaticLeave
        text: "Выход через " + _closeWindowTimer.counter.toString() + "..."
        anchors.margins: _win.defaultMargin/2
        anchors.left: root.left
        anchors.bottom: root.bottom
        color: Qt.lighter(root.color, 1.3)
        style: Text.Outline
        font.pixelSize: root.baseHeight/15
    }

    Timer
    {
        id: _closeWindowTimer
        interval: 1000
        property int counter: 20
        repeat: true
        onTriggered:
        {
            counter--;

            if(counter <= -1)
                _win.visibility = Window.Hidden
        }
    }

    function startCounting()
    {
        _closeWindowTimer.start();
    }

    function prepareTopPlayersModel()
    {
        _topPlayersModel.clear();
        for(let i = 0; i < _playersInfoModel.rowCount(); i++)
        {
            let item = {};
            item.playerName   = _playersInfoModel.data(_playersInfoModel.index(i, 0),
                                                       Helper.PlayerModelRole.DisplayableNameRole);
            item.playerNumber = _playersInfoModel.data(_playersInfoModel.index(i, 0),
                                                       Helper.PlayerModelRole.PlayerNumberRole);
            _topPlayersModel.append(item);
        }
    }

    ListModel
    {
        id: _topPlayersModel
    }
}
