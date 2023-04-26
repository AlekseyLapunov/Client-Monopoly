import QtQuick 2.15
import QtQuick3D
import QtMultimedia
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
        id: _view3D
        anchors.fill: root

        PerspectiveCamera
        {
            id: _camera

            property vector3d closePosition: Qt.vector3d(2.5, 0, 50)
            property vector3d abovePosition: Qt.vector3d(30, 38, 45)
            property vector3d closeLookRotation: Qt.vector3d(0, 0, 0)
            property vector3d aboveLookRotation: Qt.vector3d(-37, 25, 0)

            position: abovePosition
            eulerRotation: aboveLookRotation

            Behavior on position
            {
                PropertyAnimation
                {
                    easing.period: 0.42
                    easing.type: Easing.OutElastic
                    duration: 2000
                }
            }

            Behavior on eulerRotation
            {
                PropertyAnimation
                {
                    easing.period: 0.42
                    easing.type: Easing.OutElastic
                    duration: 2000
                }
            }
        }

        environment: SceneEnvironment
        {
            antialiasingMode: SceneEnvironment.MSAA
            antialiasingQuality: SceneEnvironment.High
        }

        MyDice3D
        {
            id: _leftDice
            basePosition: Qt.vector3d(-16, 0, 0)
            diceColor: leftDiceColor
            whatDice: Helper.Dice.Left
        }

        MyDice3D
        {
            id: _rightDice
            basePosition: Qt.vector3d(22, 0, 0)
            diceColor: rightDiceColor
            whatDice: Helper.Dice.Right
        }

        property double pointLightBrightness: 0.85

        PointLight
        {
            id: _lightPointXPos
            x: 80; y: 0; z: 0
            brightness: _view3D.pointLightBrightness
        }

        PointLight
        {
            id: _lightPointXNeg
            x: -80; y: 0; z: 0
            brightness: _view3D.pointLightBrightness
        }

        PointLight
        {
            id: _lightPointYPos
            x: 0; y: 40; z: 0
            brightness: _view3D.pointLightBrightness
        }

        PointLight
        {
            id: _lightPointYNeg
            x: 0; y: -40; z: 0
            brightness: _view3D.pointLightBrightness
        }

        PointLight
        {
            id: _lightPointZPos
            x: 0; y: 0; z: 40
            brightness: _view3D.pointLightBrightness
        }

        PointLight
        {
            id: _lightPointZNeg
            x: 0; y: 0; z: -40
            brightness: _view3D.pointLightBrightness
        }
    }

    SoundEffect
    {
        id: _diceOutSoundEffect
        source: "../../assets/sounds/dice/dice_out.wav"
        volume: 0.30
    }

    function cameraClosePosition()
    {
        _camera.position = _camera.closePosition;
        _camera.eulerRotation = _camera.closeLookRotation;
    }

    function cameraAbovePosition()
    {
        _camera.position = _camera.abovePosition;
        _camera.eulerRotation = _camera.aboveLookRotation;
    }

    function diceStop()
    {
        _leftDice.stopInfiniteRotation();
        _rightDice.stopInfiniteRotation();
    }

    function diceGoInfiniteRotation()
    {
        cameraAbovePosition();
        _leftDice.resumeInfiniteRotation();
        _rightDice.resumeInfiniteRotation();
    }

    function doDirectRotateTo(whatDice: int, diceNumber: int)
    {
        if(whatDice === Helper.Dice.Left)
            _leftDice.doDirectionalRotate(diceNumber);
        else
            _rightDice.doDirectionalRotate(diceNumber);
    }

    function diceRotateToNumbers(leftDiceVal: int, rightDiceVal: int)
    {
        cameraClosePosition();
        _leftDice.doDirectionalRotate(leftDiceVal);
        _rightDice.doDirectionalRotate(rightDiceVal);
        _diceOutSoundEffect.play();
    }
}
