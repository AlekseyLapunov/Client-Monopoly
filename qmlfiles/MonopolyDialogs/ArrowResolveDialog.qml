import QtQuick 2.15
import "../HelperSingletone"
import "../MonopolyComponents/InfoBlocks"
import "../MonopolyComponents"
import "../MonopolyComponents/SubComponents"

MonopolyDialog
{
    id: root

    property int arrowCellIndex
    property int currentArrowDirection
    property int allowedArrowDirections
    property int arrowRotateCost

    property int updatedAllowedDirectionsMask

    dialogWindowTitle: "Поворот стрелки"
    hasCloseButton: false
    canBeDragged: true

    heightCoeff: 1.2
    widthCoeff: 1.3

    onVisibleChanged:
    {
        if(root.visible !== true)
            return;

        _acceptRotationButton.state = "disabled";
        updateAllowedDirectionsMask();
        _arrowCell.updateDisplayedCellInfo();
    }

    Cell
    {
        id: _arrowCell

        anchors.centerIn: root

        height: root.height*0.40
        width: height

        ignoreClickForDialog: true

        displayableFieldType: Helper.FieldType.Arrow
        arrowDirection: root.currentArrowDirection
        border.width: 2
        border.color: Helper.applyContrast(root.color, 1.8)

        MouseArea
        {
            id: _rotateArea
            anchors.fill: _arrowCell
            cursorShape: Qt.PointingHandCursor
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            z: root.z + 1
            onClicked: (mouse)=>
            {
                switch(mouse.button)
                {
                case Qt.LeftButton:
                    if(_arrowCell.arrowDirection === Helper.ArrowRotation.ArrowLeft)
                        _arrowCell.arrowDirection = Helper.ArrowRotation.ArrowUp;
                    else
                        _arrowCell.arrowDirection++;
                    break;
                case Qt.RightButton:
                    if(_arrowCell.arrowDirection === Helper.ArrowRotation.ArrowUp)
                        _arrowCell.arrowDirection = Helper.ArrowRotation.ArrowLeft;
                    else
                        _arrowCell.arrowDirection--;
                    break;
                default:
                    break;
                }
                checkAcceptButtonState();
            }
        }

        function updateDisplayedCellInfo()
        {
            _arrowCell.arrowDirection = root.currentArrowDirection;
        }
    }

    Rectangle
    {
        id: _hintBackground
        color: Qt.darker(root.color, 1.1)
        anchors.top: _arrowCell.top
        anchors.bottom: _arrowCell.bottom
        anchors.left: root.left
        anchors.right: _arrowCell.left
        anchors.leftMargin: _win.defaultMargin
        anchors.rightMargin: _win.defaultMargin

        Text
        {
            id: _hintText
            anchors.centerIn: _hintBackground
            text: "Левая клавиша мыши:\nповорот по\nчасовой стрелке\n\nПравая клавиша мыши:\nповорот против\nчасовой стрелки"
            color: Qt.lighter(_hintBackground.color, 1.7)
            horizontalAlignment: Text.AlignHCenter
            font.family: "Bookman Old Style"
            font.pixelSize: _hintBackground.height/8.2
            style: Text.Outline
            fontSizeMode: Text.Fit
        }
    }

    Rectangle
    {
        id: _costBackground
        color: _hintBackground.color
        anchors.top: _arrowCell.top
        anchors.bottom: _arrowCell.bottom
        anchors.left: _arrowCell.right
        anchors.right: root.right
        anchors.leftMargin: _win.defaultMargin
        anchors.rightMargin: _win.defaultMargin

        Text
        {
            id: _costText
            anchors.centerIn: _costBackground
            text: ("Стоимость поворота:\n" + root.arrowRotateCost.toString() + " млн. ЭК")
            color: Qt.lighter(_costBackground.color, 1.7)
            horizontalAlignment: Text.AlignHCenter
            font.family: "Bookman Old Style"
            font.pixelSize: _costBackground.height/7
            style: Text.Outline
            fontSizeMode: Text.Fit
        }
    }

    Rectangle
    {
        id: _buttonsSeparator
        color: "transparent"
        height: root.height/10
        width: height/10
        anchors.bottom: root.bottom
        anchors.horizontalCenter: root.horizontalCenter
    }

    MonopolyButton
    {
        id: _acceptRotationButton

        textContent: "Принять"
        height: root.baseHeight/5
        width: height*3
        anchors.right: _buttonsSeparator.left
        anchors.bottom: root.bottom
        anchors.margins: _win.defaultMargin/3
        sharedColor: Qt.lighter(root.color, 1.1)
        hasText: true
        state: "normal"
        z: 5

        onClicked:
        {
            _win.arrowResolveResult(root.arrowCellIndex, _arrowCell.arrowDirection);
            root.visible = false;
        }

    }

    MonopolyButton
    {
        id: _closeAndDenyButton

        textContent: "Отказаться"
        height: _acceptRotationButton.height
        width: _acceptRotationButton.width
        anchors.left: _buttonsSeparator.right
        anchors.bottom: root.bottom
        anchors.margins: _win.defaultMargin/3
        sharedColor: Qt.lighter(root.color, 1.1)
        hasText: true
        state: "normal"
        z: 5

        onClicked:
        {
            _win.arrowResolveResult(root.arrowCellIndex, -1);
            root.visible = false;
        }
    }

    function updateAllowedDirectionsMask()
    {
        let nextRotation = root.currentArrowDirection

        do
        {
            if(nextRotation === Helper.ArrowRotation.ArrowLeft)
                nextRotation = Helper.ArrowRotation.ArrowUp
            else nextRotation++;

            if((root.allowedArrowDirections & (1 << nextRotation)) !== 0)
                break;
        }
        while(nextRotation !== root.currentArrowDirection);

        let maskApplier = ~(1 << nextRotation);
        maskApplier &= ~(1 << root.currentArrowDirection);

        updatedAllowedDirectionsMask = root.allowedArrowDirections;

        root.updatedAllowedDirectionsMask &= maskApplier;
    }

    function checkAcceptButtonState()
    {
        if((root.updatedAllowedDirectionsMask & (1 << _arrowCell.arrowDirection)) !== 0)
            _acceptRotationButton.state = "normal";
        else
            _acceptRotationButton.state = "disabled";
    }
}
