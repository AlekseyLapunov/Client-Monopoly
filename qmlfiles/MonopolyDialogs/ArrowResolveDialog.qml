import QtQuick 2.15
import "../HelperSingletone"
import "../MonopolyComponents/InfoBlocks"
import "../MonopolyComponents"
import "../MonopolyComponents/SubComponents"

MonopolyDialog
{
    id: root

    property int currentArrowDirection
    property int allowedArrowDirections
    property int arrowRotateCost

    dialogWindowTitle: "Поворот стрелки"
    hasCloseButton: false
    canBeDragged: true

    Cell
    {
        id: _arrowCell

        anchors.centerIn: root

        height: root.height*0.5
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

            onClicked:
            {
                switch(mouse.button)
                {
                case Qt.LeftButton:
                    if(_arrowCell.arrowDirection === Helper.ArrowRotation.ArrowLeft)
                        _arrowCell.arrowDirection = Helper.ArrowRotation.ArrowUp;
                    else
                        _arrowCell.arrowDirection++;
                    return;
                case Qt.RightButton:
                    if(_arrowCell.arrowDirection === Helper.ArrowRotation.ArrowUp)
                        _arrowCell.arrowDirection = Helper.ArrowRotation.ArrowLeft;
                    else
                        _arrowCell.arrowDirection--;
                    return;
                default:
                    return;
                }
            }
            z: root.z + 1
        }
    }
}
