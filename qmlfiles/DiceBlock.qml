import QtQuick 2.15
import QtQuick3D

Rectangle
{
    id: root
    color: "lightgrey"

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
            position: Qt.vector3d(-15, 0, 0)
            PrincipledMaterial
            {
                baseColor: _displayField.makeRgb(255, 0, 0)
            }
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
            PrincipledMaterial
            {
                baseColor: _displayField.makeRgb(0, 255, 0)
            }
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
