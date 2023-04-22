import QtQuick 2.15
import QtQuick3D
import "SubComponents"
import "../HelperSingletone"

Rectangle
{
    id: root
    property color sharedColor: Qt.rgba(0, 0, 0, 0)
    property color baseColor: Qt.rgba(sharedColor.r-0.25,
                                      sharedColor.g-0.25,
                                      sharedColor.b-0.25)
    property color leftDiceColor: Qt.rgba(0, 0, 0, 0)
    property color rightDiceColor: Qt.rgba(0, 0, 0, 0)
    color: baseColor
    radius: (height + width)*_win.componentsRadiusCoeff
    border.width: (height+width)*_win.componentsBorderCoeff
    border.color: Qt.lighter(root.color, 1.2)

    View3D
    {
        anchors.fill: root

        PerspectiveCamera
        {
            id: _camera
            position: Qt.vector3d(2.5, 0, 50);
        }
        DirectionalLight
        {
            id: _dl
            brightness: 1
        }

        MyDice3D
        {
            position: Qt.vector3d(-16, 0, 0)
            diceColor: leftDiceColor
            PropertyAnimation on eulerRotation
            {
                loops: Animation.Infinite
                from: Qt.vector3d(0, 0, 0)
                to: Qt.vector3d(360, 360, 360)
                duration: 1500
            }
        }

        MyDice3D
        {
            position: Qt.vector3d(22, 0, 0)
            diceColor: rightDiceColor
            PropertyAnimation on eulerRotation
            {
                loops: Animation.Infinite
                from: Qt.vector3d(0, 0, 0)
                to: Qt.vector3d(360, 360, 360)
                duration: 1500
            }
        }
   }
}
