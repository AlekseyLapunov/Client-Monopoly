import QtQuick 2.15
import QtMultimedia
import Qt5Compat.GraphicalEffects
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

        onVisibleChanged:
        {
            _acLevitateAnimation1.restart();
            _acLevitateAnimation2.restart();
        }

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

        Rectangle
        {
            id: _acRectangleCenter1

            anchors.horizontalCenter: _imagePlaceholder.horizontalCenter

            Image
            {
                id: _acImage1

                opacity: 0.8

                property int localSizeRatio
                property int localSizeUnit: (_imagePlaceholder.width + _imagePlaceholder.height)*1.1

                anchors.centerIn: _acRectangleCenter1

                source: (stageNumber === 1) ? "../../assets/svgs/stage_components/stage_additional_stage_1.svg" :
                        (stageNumber === 2) ? "../../assets/svgs/stage_components/stage_additional_stage_2.svg" :
                        (stageNumber === 3) ? "../../assets/svgs/stage_components/stage_additional_stage_3.svg" :
                                              ""

                Component.onCompleted:
                {
                    _acImage1.localSizeRatio =
                            (_acImage1.sourceSize.height / _acImage1.sourceSize.width);
                    _acLevitateAnimation1.start()
                }

                sourceSize.width: localSizeUnit
                sourceSize.height: localSizeUnit*localSizeRatio

                z: 4

                ColorOverlay
                {
                    id: _acImageColorOverlay
                    source: _acImage1
                    anchors.fill: _acImage1
                    color: Qt.rgba(_displayField.shareGradColor2.r,
                                   _displayField.shareGradColor2.g,
                                   _displayField.shareGradColor2.b,
                                   0.4)
                }
            }

            PropertyAnimation
            {
                property double levitateOrigin: 150
                property double levitateUpY: levitateOrigin - 50
                property double levitateDownY: levitateOrigin + 100
                property bool toggler: true

                id: _acLevitateAnimation1
                easing.type: Easing.OutQuad
                property: "y"
                target: _acRectangleCenter1
                from: levitateOrigin
                to: toggler ? levitateUpY
                            : levitateDownY
                duration: 4000

                onFinished:
                {
                    _acLevitateAnimation1.from = _acRectangleCenter1.y
                    _acLevitateAnimation1.toggler = !_acLevitateAnimation1.toggler
                    _acLevitateAnimation1.start()
                }
            }
        }

        Rectangle
        {
            id: _acRectangleCenter2

            anchors.horizontalCenter: _imagePlaceholder.horizontalCenter

            Image
            {
                id: _acImage2

                opacity: 0.5

                anchors.centerIn: _acRectangleCenter2

                property int localSizeRatio
                property int localSizeUnit: (_imagePlaceholder.width + _imagePlaceholder.height)*0.7
                source: (stageNumber === 1) ? "../../assets/svgs/stage_components/stage_additional_stage_1.svg" :
                        (stageNumber === 2) ? "../../assets/svgs/stage_components/stage_additional_stage_2.svg" :
                        (stageNumber === 3) ? "../../assets/svgs/stage_components/stage_additional_stage_3.svg" :
                                              ""

                Component.onCompleted:
                {
                    _acImage2.localSizeRatio =
                            (_acImage2.sourceSize.height / _acImage2.sourceSize.width);
                    _acLevitateAnimation2.start()
                }

                sourceSize.width: localSizeUnit
                sourceSize.height: localSizeUnit*localSizeRatio

                z: 4

                ColorOverlay
                {
                    id: _acImageColorOverlay2
                    source: _acImage2
                    anchors.fill: _acImage2
                    color: Qt.rgba(_displayField.shareGradColor1.r,
                                   _displayField.shareGradColor1.g,
                                   _displayField.shareGradColor1.b,
                                   0.4)
                }
            }

            PropertyAnimation
            {
                property double levitateOrigin: 150
                property double levitateUpY: levitateOrigin
                property double levitateDownY: levitateOrigin + 100
                property bool toggler: false

                id: _acLevitateAnimation2
                property: "y"
                easing.type: Easing.OutQuad
                target: _acRectangleCenter2
                from: levitateOrigin
                to: toggler ? levitateUpY
                            : levitateDownY
                duration: 4500

                onFinished:
                {
                    _acLevitateAnimation2.from = _acRectangleCenter2.y
                    _acLevitateAnimation2.toggler = !_acLevitateAnimation2.toggler
                    _acLevitateAnimation2.start()
                }
            }
        }

        Behavior on opacity { PropertyAnimation { duration: 800 } }
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
    }

    Timer
    {
        id: _mainTimer
        interval: _win.stageAnimationDurationMs + 400
        repeat: false

        onTriggered:
        {
            root.stopStageAnimation();
        }
    }

    Timer
    {
        id: _secondaryTimer
        interval: _win.stageAnimationDurationMs - 400
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
