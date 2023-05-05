import QtQuick 2.15
import QtMultimedia
import "../HelperSingletone"
import "SubComponents"

Rectangle
{
    id: root

    property int stageNumber: _win.currentStage

    property string soundSource: (stageNumber === 1) ? "../../assets/sounds/stage_openings/stage_1_opening.wav" :
                                 (stageNumber === 2) ? "../../assets/sounds/stage_openings/stage_2_opening.wav" :
                                 (stageNumber === 3) ? "../../assets/sounds/stage_openings/stage_3_opening.wav" :
                                                       ""

    visible: false
    color: Qt.rgba(_displayField.shareGradColor1.r-0.2,
                   _displayField.shareGradColor1.g-0.2,
                   _displayField.shareGradColor1.b-0.2,
                   0.35).toString()

    Rectangle
    {
        id: _imagePlaceholder

        anchors.centerIn: root
        width: root.width/3
        height: root.height/3
        color: "transparent"
        visible: root.visible
        opacity: 0.0

        StageChangingImageAnimated
        {
            id: _stageAnimated

            anchors.centerIn: _imagePlaceholder

            Text
            {
                id: _stageText
                anchors.centerIn: _stageAnimated
                text: ("Стадия " + stageNumber.toString())
                color: Helper.applyContrast(_displayField.shareGradColor2, 1.2)
                style: Text.Outline
                styleColor: Qt.lighter(color, 1.3)
                font.family: "Bookman Old Style"
                font.pointSize: Helper.fontWarningPreventor(root.width*0.045)
                z: 5
            }
        }

        Behavior on opacity { PropertyAnimation { duration: 800 } }
    }

    Rectangle
    {
        id: _acCircleBase
        anchors.centerIn: root
        width: root.width/3.5
        height: width
        radius: width/2.2
        color: "transparent"
        rotation: (_clickSilencerMouseArea.mouseX + _clickSilencerMouseArea.mouseY)
    }

    Rectangle
    {
        id: _acRectangleCenter
        width: _acCircleBase.width/10
        height: width
        color: "transparent"

        property double centerX: (_acCircleBase.x + _acCircleBase.width/2 - _acRectangleCenter.width/2)
        property double centerY: (_acCircleBase.y + _acCircleBase.height/2 - _acRectangleCenter.height/2)

        x: centerX + Math.cos(_acCircleBase.rotation/100)*260
        y: centerY + Math.sin(_acCircleBase.rotation/100)*260

        Image
        {
            id: _additionalComponentsImage

            property int localSizeRatio
            property int localSizeUnit: (_imagePlaceholder.width + _imagePlaceholder.height)*1

            anchors.centerIn: _acRectangleCenter
            source: (stageNumber === 1) ? "../../assets/svgs/stage_components/stage_additional_stage_1.svg" :
                    (stageNumber === 2) ? "../../assets/svgs/stage_components/stage_additional_stage_2.svg" :
                    (stageNumber === 3) ? "../../assets/svgs/stage_components/stage_additional_stage_3.svg" :
                                          ""

            Component.onCompleted:
            {
                _additionalComponentsImage.localSizeRatio =
                        (_additionalComponentsImage.sourceSize.height / _additionalComponentsImage.sourceSize.width);
            }

            sourceSize.width: localSizeUnit
            sourceSize.height: localSizeUnit*localSizeRatio

            z: 4
        }
    }

    Behavior on opacity { PropertyAnimation { duration: 400 } }

    SoundEffect
    {
        id: _stageOpeningSound
        source: soundSource
        volume: 1
    }

    MouseArea
    {
        id: _clickSilencerMouseArea
        anchors.fill: root
        enabled: root.visible
        hoverEnabled: true

        HoverHandler
        {
            id: _hoverHandler
            enabled: _clickSilencerMouseArea.enabled
        }
    }

    Timer
    {
        id: _mainTimer
        interval: 5500
        repeat: false

        onTriggered:
        {
            root.stopStageAnimation();
        }
    }

    Timer
    {
        id: _secondaryTimer
        interval: 4700
        repeat: false

        onTriggered:
        {
            _imagePlaceholder.opacity = 0.0
            root.opacity = 0.0
        }
    }

    function startStageAnimation()
    {
        _mainTimer.stop();
        prepareForAnimation();

        _mainTimer.start();
        _secondaryTimer.start();
        _stageOpeningSound.play();
        root.visible = true;
        root.opacity = 1.0
        _imagePlaceholder.opacity = 1.0
    }

    function stopStageAnimation()
    {
        _mainTimer.stop();
        prepareForAnimation();
    }

    function prepareForAnimation()
    {
        root.visible = false;
        _imagePlaceholder.opacity = 0.0
        root.opacity = 0.0
    }
}
