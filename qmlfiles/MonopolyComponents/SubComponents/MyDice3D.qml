import QtQuick 2.15
import QtQuick3D
import QtQuick3D.Helpers
import "../../HelperSingletone"

Node
{
    position: Qt.vector3d(0, 0, 0)
    property color diceColor: "white"

    PrincipledMaterial
    {
        id: _priDiceMaterial
        baseColorMap: Texture
        {
            source: "../../../assets/3d_dice/Dice_Diffuse.png"
        }
        baseColor: Helper.applyContrast(diceColor, 1.4)
    }

    Model
    {
       id: _firstDiceModel
       source: "../../../assets/3d_dice/dice_001.mesh"
       materials: [_priDiceMaterial]
       z: -10
    }

    PropertyAnimation on eulerRotation
    {
        id: _infiniteRotationAnimation
        property vector3d lastKnownAngle
        from: Qt.vector3d(0, 0, 0)
        to: Helper.getRandomQtVector(0, 360)
        duration: 600
        onFinished:
        {
            if(_infiniteRotationAnimation.paused)
                return;

            let lastKnownAngle = _infiniteRotationAnimation.to;

            _infiniteRotationAnimation.from = lastKnownAngle;
            _infiniteRotationAnimation.to = Helper.getRandomQtVector(0, 360);

            _infiniteRotationAnimation.restart();
        }
    }

//    PropertyAnimation on eulerRotation
//    {
//        id: _directionalRotationAnimation
//        loops: Animation.Infinite
//        from: Qt.vector3d(0, 0, 0)
//        to: Qt.vector3d(360, 360, 360)
//        duration: 1500
//    }
}
