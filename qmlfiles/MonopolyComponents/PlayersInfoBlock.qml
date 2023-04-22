import QtQuick 2.15
import "../HelperSingletone"

Rectangle
{
    id: root
    property color sharedColor: Qt.rgba(0, 0, 0, 0)
    property color baseColor: Qt.rgba(sharedColor.r-0.25,
                                      sharedColor.g-0.25,
                                      sharedColor.b-0.25)

    property int dataRowHeight: sizeUnit*0.09
    property int dataRowWidth: sizeUnit*0.90

    radius: (height/Math.pow(width, 1.2))*30

    height: (dataRowHeight+_dataRows.spacing)*_playersDataRows.count + sizeUnit/6
    color: baseColor

    border.width: (height+width)*_win.componentsBorderCoeff/3
    border.color: Qt.lighter(root.color, 1.2)

    Text
    {
        id: _blockLabel
        anchors.top: root.top
        anchors.topMargin: _win.defaultMargin/8
        anchors.horizontalCenter: root.horizontalCenter
        text: "Игроки"
        font.bold: true
        font.underline: true
        color: Qt.lighter(root.color, 1.7)
        font.family: "Bookman Old Style"
        font.pointSize: root.width*0.055
    }

    Column
    {
        id: _dataRows
        anchors.top: _blockLabel.bottom
        anchors.horizontalCenter: root.horizontalCenter
        anchors.topMargin: _win.defaultMargin/8
        spacing: sizeUnit/80
        move: Transition {
            NumberAnimation { properties: "x,y"; duration: 800 }
        }
        Repeater
        {
            model: _playersDataRows

            delegate: Rectangle
            {
                height: dataRowHeight
                width: dataRowWidth
                color: ((index%2 === 0) ? Qt.lighter(root.color, 1.15)
                                        : Qt.darker(root.color, 1.15))

                property color playerColorProp: playerColor
                property string playerNicknameProp: playerNickname
                property int playerBalanceProp: playerBalance

                Text
                {
                    id: _nicknameLabel
                    anchors.left: parent.left
                    anchors.leftMargin: _win.defaultMargin/8
                    anchors.verticalCenter: parent.verticalCenter
                    text: parent.playerNicknameProp
                    font.bold: true
                    color: Qt.lighter(parent.playerColorProp, 1.1)
                    font.family: "Bookman Old Style"
                    font.pointSize: parent.height*0.4
                    style: Text.Outline
                    styleColor: (parent.index%2 === 0) ? Qt.darker(root.color, 1.25)
                                                       : Qt.lighter(root.color, 1.05)
                }

                Text
                {
                    id: _moneyLabel
                    anchors.right: parent.right
                    anchors.rightMargin: _win.defaultMargin/8
                    anchors.verticalCenter: parent.verticalCenter
                    text: parent.playerBalanceProp.toString() + " млн."
                    font.bold: true
                    color: Qt.lighter(parent.playerColorProp, 1.1)
                    font.family: "Bookman Old Style"
                    font.pointSize: parent.height*0.5
                    style: Text.Outline
                    styleColor: (parent.index%2 === 0) ? Qt.darker(root.color, 1.25)
                                                       : Qt.lighter(root.color, 1.05)
                }

                Behavior on color { PropertyAnimation { duration: 500 } }
            }
        }
    }

    function addPlayerRow()
    {
        if(_playersDataRows.count === 6)
            return;

        let playerDataItem = {};
        playerDataItem.playerColor = Helper.definePlayerColorByNumber(_playersDataRows.count+1).toString();
        playerDataItem.playerBalance = _playersDataRows.count === 0 ? Math.random()*2000 + 1000
                                                                    : _playersDataRows.get(_playersDataRows.count - 1).playerBalance
                                                                    - (Math.random()*300 + 100);
        playerDataItem.playerNickname = "Player" + (_playersDataRows.count + 1);
        _playersDataRows.append(playerDataItem);
    }

    function debugChangePlayerBalance()
    {
        _playersDataRows.get(3).playerBalance += 1000;

        sortPlayersList();
    }

    function delPlayerRow()
    {
        if(_playersDataRows.count === 0)
            return;
        _playersDataRows.remove(_playersDataRows.count - 1);
    }

    function sortPlayersList()
    {

        if(_playersDataRows.count < 2)
            return;

        for (let i = 0; i < (_playersDataRows.count - 1); i++)
            for (let j = 0; j < (_playersDataRows.count - i - 1); j++)
                if (_playersDataRows.get(j).playerBalance < _playersDataRows.get(j + 1).playerBalance)
                {
                    _playersDataRows.move(j, j+1, 1)
                    //_playersDataRows.move(j+1, j, 1)
                }
    }

    ListModel
    {
        id: _playersDataRows
    }
}
