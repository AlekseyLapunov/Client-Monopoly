import QtQuick 2.15
import QtMultimedia
import "../../HelperSingletone"

Rectangle
{
    id: root
    property color sharedColor: Qt.rgba(0, 0, 0, 0)
    property color baseColor: Qt.rgba(sharedColor.r-0.25,
                                      sharedColor.g-0.25,
                                      sharedColor.b-0.25)

    property int dataRowHeight: sizeUnit*0.12
    property int dataRowWidth: sizeUnit*0.98

    radius: (height/Math.pow(width, 1.2))*30

    height: (dataRowHeight+_dataRows.spacing)*_playersDataRowsSortable.count + sizeUnit/6
    color: baseColor

    border.width: (height+width)*_win.componentsBorderCoeff/3
    border.color: Qt.lighter(root.color, 1.2)

    Text
    {
        id: _blockLabel
        anchors.top: root.top
        anchors.topMargin: _win.defaultMargin/8
        anchors.horizontalCenter: root.horizontalCenter
        text: Helper.fontWarningPreventor("Игроки")
        font.bold: true
        font.underline: true
        color: Qt.lighter(root.color, 1.7)
        font.family: "Bookman Old Style"
        font.pointSize: Helper.fontWarningPreventor(root.width*0.055)
    }

    SoundEffect
    {
        id: _energyKrendelsOvercomeSound
        source: "../../../assets/sounds/players_list/energy_krendels_overcome.wav"
        volume: 0.63
    }

    Column
    {
        id: _dataRows
        anchors.top: _blockLabel.bottom
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
            model: _playersDataRowsSortable

            delegate: Rectangle
            {
                height: dataRowHeight
                width: dataRowWidth
                color: ((index%2 === 0) ? Qt.lighter(root.color, 1.15)
                                        : Qt.darker(root.color, 1.15))

                property color playerColorProp: Helper.definePlayerColorByNumber(model.playerNumber)
                property string playerNicknameProp: model.displayableName
                property int playerBalanceProp: model.currentBalance

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
                    font.pointSize: parent.playerNicknameProp.length <= 8 ?
                                        Helper.fontWarningPreventor(parent.height*0.35) :
                                        Helper.fontWarningPreventor(parent.height*0.292)
                    style: Text.Outline
                    styleColor: Qt.darker(root.color, 1.55)
                }

                Text
                {
                    id: _moneyLabel
                    anchors.right: parent.right
                    anchors.rightMargin: _win.defaultMargin/8
                    anchors.verticalCenter: parent.verticalCenter
                    text: parent.playerBalanceProp.toString() + " млн. ЭК"
                    font.bold: true
                    color: Qt.lighter(parent.playerColorProp, 1.1)
                    font.family: "Bookman Old Style"
                    font.pointSize: Helper.fontWarningPreventor(parent.height*0.35)
                    style: Text.Outline
                    styleColor: Qt.darker(root.color, 1.55)
                }

                Behavior on color { PropertyAnimation { duration: 500 } }
            }
        }
    }

    Behavior on height
    {
        PropertyAnimation
        {
            easing.type: Easing.OutQuad
            duration: 600
        }
    }

    Component.onCompleted:
    {
        fillPlayersSortable();
    }

    function sortPlayersList()
    {
        if(_playersDataRowsSortable.count < 2)
            return;

        for (let i = 0; i < (_playersDataRowsSortable.count - 1); i++)
            for (let j = 0; j < (_playersDataRowsSortable.count - i - 1); j++)
                if (_playersDataRowsSortable.get(j).currentBalance < _playersDataRowsSortable.get(j + 1).currentBalance)
                {
                    _playersDataRowsSortable.move(j, j+1, 1);
                    _energyKrendelsOvercomeSound.play();
                }
    }

    function fillPlayersSortable()
    {
        _playersDataRowsSortable.clear();
        for(let i = 0; i < _playersInfoModel.rowCount(); i++)
        {
            let item = {};
            item.playerNumber = _playersInfoModel.data(_playersInfoModel.index(i, 0),
                                                       Helper.PlayerModelRole.PlayerNumberRole);
            item.displayableName = _playersInfoModel.data(_playersInfoModel.index(i, 0),
                                                          Helper.PlayerModelRole.DisplayableNameRole);
            item.currentBalance = _playersInfoModel.data(_playersInfoModel.index(i, 0),
                                                         Helper.PlayerModelRole.CurrentBalanceRole);
            _playersDataRowsSortable.append(item);
        }
        sortPlayersList();
    }

    function updatePlayersSortableNickname(playerNumber, newNickname)
    {
        if(playerNumber > _playersDataRowsSortable.count)
            return;

        for(let i = 0; i < _playersDataRowsSortable.count; i++)
        {
            if(_playersDataRowsSortable.get(i).playerNumber === playerNumber)
            {
                _playersDataRowsSortable.get(i).displayableName = newNickname;
                return;
            }
        }
    }

    function updatePlayersSortableBalance(playerNumber, newBalance)
    {
        if(playerNumber > _playersDataRowsSortable.count)
            return;

        for(let i = 0; i < _playersDataRowsSortable.count; i++)
        {
            if(_playersDataRowsSortable.get(i).playerNumber === playerNumber)
            {
                _playersDataRowsSortable.get(i).currentBalance = newBalance;
                sortPlayersList();
                return;
            }
        }
    }

    ListModel
    {
        id: _playersDataRowsSortable
    }

    Connections
    {
        target: _gameTransmitter

        function onAppendPlayer(playerNumber, displayableName, currentBalance)
        {
            let item = {};
            item.playerNumber = playerNumber;
            item.displayableName = displayableName;
            item.currentBalance = currentBalance;
            _playersDataRowsSortable.append(item);
            _playersList.appendItem(playerNumber, displayableName, currentBalance, 0);
            console.log("Appending Player" + playerNumber.toString() + " (" + displayableName + ")");
            sortPlayersList();
        }

        function onDeletePlayer(playerNumber)
        {
            for(let i = 0; i < _playersDataRowsSortable.count; i++)
            {
                if(_playersDataRowsSortable.get(i).playerNumber === playerNumber)
                {
                    console.log("Deleting Player" + _playersDataRowsSortable.get(i).playerNumber
                                + " (" + _playersDataRowsSortable.get(i).displayableName + ")");
                    _playersDataRowsSortable.remove(i);
                    break;
                }
            }
            _playersList.removeItem(playerNumber);
            sortPlayersList();
        }
    }
}
