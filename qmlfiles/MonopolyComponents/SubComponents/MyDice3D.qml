import QtQuick 2.15
import QtQuick3D
import QtQuick3D.Helpers
import "../../HelperSingletone"

Node
{
    id: root

    property vector3d basePosition: Qt.vector3d(0, 0, 0)

    position: basePosition
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
        //easing.amplitude: 2.5
        //easing.type: Easing.InOutExpo
        from: root.eulerRotation
        to: Helper.getRandomQtVector(0, 360)
        duration: Helper.getRandomInt(300, 550)
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

    PropertyAnimation on position
    {
        id: _levitateAnimation
        property vector3d levitateUpVector: Qt.vector3d(0, 3, 0)
        property vector3d levitateDownVector: Qt.vector3d(0, -3, 0)
        property bool toggler: (root.whatDice === Helper.Dice.Left)
        easing.type: Easing.InOutQuad
        duration: 600
        from: position
        to: toggler ? root.basePosition.plus(levitateUpVector)
                    : root.basePosition.plus(levitateDownVector)
        onFinished:
        {
            _levitateAnimation.toggler = !_levitateAnimation.toggler;
            _levitateAnimation.start();
        }
    }

    Behavior on position { PropertyAnimation { duration: 300 } }

    function stopInfiniteRotation()
    {
        _levitateAnimation.stop();
        root.position = root.basePosition;
        _infiniteRotationAnimation.stop();
    }

    function resumeInfiniteRotation()
    {
        _levitateAnimation.restart();
        _infiniteRotationAnimation.restart();
    }

    function doDirectionalRotate(diceNumber: int)
    {
        if(_infiniteRotationAnimation.running)
        {
            _infiniteRotationAnimation.pause();
            _levitateAnimation.stop();
            root.position = root.basePosition;
        }
        _directionalRotationAnimation.to = Helper.getDirectionalRotate(diceNumber,
                                                                       whatDice);
        _directionalRotationAnimation.duration = 2500;
        _directionalRotationAnimation.restart();
    }
}
