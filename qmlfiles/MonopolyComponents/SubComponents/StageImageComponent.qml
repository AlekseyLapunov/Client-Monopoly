import QtQuick 2.15
import Qt5Compat.GraphicalEffects

Image
{
    id: root

    property int localSizeRatio
    property int localSizeUnit
    property string imageSource: ""

    source: imageSource

    sourceSize.width: localSizeUnit
    sourceSize.height: localSizeUnit*localSizeRatio

    RotationAnimator
    {
       id: _rotationAnimation
       target: root
       duration: 0
    }

    ColorOverlay
    {
        id: _colorOverlay
        source: root
        anchors.fill: root
        color: Qt.rgba(_displayField.shareGradColor2.r-0.2,
                       _displayField.shareGradColor2.g-0.2,
                       _displayField.shareGradColor2.b-0.2,
                       0.8).toString()
    }

    function startRotationAnimation(durationParam: int, toParam: int)
    {
        if(toParam < 0)
            _rotationAnimation.direction = RotationAnimator.Counterclockwise;

        _rotationAnimation.duration = durationParam
        _rotationAnimation.loops = Animation.Infinite
        _rotationAnimation.from = 0
        _rotationAnimation.to = toParam
        _rotationAnimation.start()
    }
}
