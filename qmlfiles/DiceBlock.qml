import QtQuick 2.15
import QtQuick3D

Rectangle
{
    id: root
    color: Qt.rgba(_displayField.color.r-0.25,
                   _displayField.color.g-0.25,
                   _displayField.color.b-0.25)
    radius: (height + width)*0.035
    border.width: (height+width)*0.02
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
            diceColor: Helpers.makeRgb(224, 204, 255)
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
            diceColor: Helpers.makeRgb(211, 222, 237)
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
