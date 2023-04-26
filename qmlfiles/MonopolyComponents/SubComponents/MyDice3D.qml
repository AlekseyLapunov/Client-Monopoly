import QtQuick 2.15
import QtQuick3D
import QtQuick3D.Helpers
import "../../HelperSingletone"

Node
{
    id: root
    position: Qt.vector3d(0, 0, 0)
    property color diceColor: "white"
    property int whatDice

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
        property double rotationCoeff: 5
        easing.amplitude: 2.5
        easing.type: Easing.InOutExpo
        from: root.eulerRotation
        to: Helper.getRandomQtVector(0, 360)
        duration: Helper.getRandomInt(400, 750)
        onFinished:
        {
            if(_infiniteRotationAnimation.paused)
                return;

            _infiniteRotationAnimation.from = _infiniteRotationAnimation.to;
            _infiniteRotationAnimation.to = Helper.getRandomQtVector(0, 360);
            _infiniteRotationAnimation.duration = _infiniteRotationAnimation.duration

            _infiniteRotationAnimation.restart();
        }
    }

    PropertyAnimation on eulerRotation
    {
        id: _directionalRotationAnimation
        easing.type: Easing.OutElastic
        duration: 0
    }

    function stopInfiniteRotation()
    {
        _infiniteRotationAnimation.stop();
    }

    function resumeInfiniteRotation()
    {
        _infiniteRotationAnimation.restart();
    }

    function doDirectionalRotate(diceNumber: int)
    {
        _infiniteRotationAnimation.pause();
        _directionalRotationAnimation.to = Helper.getDirectionalRotate(diceNumber,
                                                                       whatDice);
        _directionalRotationAnimation.duration = 2500;
        _directionalRotationAnimation.restart();
    }
}
