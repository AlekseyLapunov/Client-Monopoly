import QtQuick 2.15
import "../SubComponents"

Rectangle
{
    id: root

    StageImageComponent
    {
        id: _mainComponent

        localSizeUnit: (_imagePlaceholder.width + _imagePlaceholder.height)*1

        source: "../../../assets/svgs/stage_components/component_0_no_rotate.svg"

        anchors.centerIn: parent

        Component.onCompleted:
        {
            _mainComponent.localSizeRatio = (_mainComponent.sourceSize.height / _mainComponent.sourceSize.width)
        }
    }

    StageImageComponent
    {
        id: _subComponent1
        localSizeRatio: _mainComponent.localSizeRatio
        localSizeUnit:  _mainComponent.localSizeUnit*0.5
        source: "../../../assets/svgs/stage_components/component_1_slow_rotate.svg"

        Component.onCompleted:
        {
            anchors.centerIn = parent
            _subComponent1.startRotationAnimation(7000, 360)
        }
    }

    StageImageComponent
    {
        id: _subComponent2
        localSizeRatio: _mainComponent.localSizeRatio
        localSizeUnit:  _mainComponent.localSizeUnit*0.3
        source: "../../../assets/svgs/stage_components/component_2_normal_rotate.svg"

        visible: (_win.currentStage > 1)

        Component.onCompleted:
        {
            anchors.centerIn = parent
            _subComponent2.startRotationAnimation(2500, -360)
        }
    }

    StageImageComponent
    {
        id: _subComponent3
        localSizeRatio: _mainComponent.localSizeRatio
        localSizeUnit:  _mainComponent.localSizeUnit*0.1
        source: "../../../assets/svgs/stage_components/component_3_fast_rotate.svg"

        visible: (_win.currentStage > 2)

        Component.onCompleted:
        {
            anchors.centerIn = parent
            _subComponent3.startRotationAnimation(1500, 360)
        }
    }

    StageImageComponent
    {
        id: _subComponent41
        localSizeRatio: _mainComponent.localSizeRatio
        localSizeUnit:  _mainComponent.localSizeUnit*0.15
        source: "../../../assets/svgs/stage_components/component_4_fastest_rotate.svg"

        visible: (_win.currentStage > 1)

        anchors.left: parent.left;
        anchors.top: parent.top;
        anchors.margins: _win.defaultMargin*(-7);

        Component.onCompleted:
        {
            _subComponent41.startRotationAnimation(600, -360);
        }
    }

    StageImageComponent
    {
        id: _subComponent42
        localSizeRatio: _mainComponent.localSizeRatio
        localSizeUnit:  _mainComponent.localSizeUnit*0.15
        source: "../../../assets/svgs/stage_components/component_4_fastest_rotate.svg"

        visible: (_win.currentStage > 2)

        anchors.right: parent.right;
        anchors.top: parent.top;
        anchors.margins: _win.defaultMargin*(-7);

        Component.onCompleted:
        {
            _subComponent42.startRotationAnimation(600, 360);
        }
    }

    StageImageComponent
    {
        id: _subComponent43
        localSizeRatio: _mainComponent.localSizeRatio
        localSizeUnit:  _mainComponent.localSizeUnit*0.15
        source: "../../../assets/svgs/stage_components/component_4_fastest_rotate.svg"

        visible: (_win.currentStage > 2)

        anchors.left: parent.left;
        anchors.bottom: parent.bottom;
        anchors.margins: _win.defaultMargin*(-7);

        Component.onCompleted:
        {
            _subComponent43.startRotationAnimation(600, -360)
        }
    }

    StageImageComponent
    {
        id: _subComponent44
        localSizeRatio: _mainComponent.localSizeRatio
        localSizeUnit:  _mainComponent.localSizeUnit*0.15
        source: "../../../assets/svgs/stage_components/component_4_fastest_rotate.svg"

        visible: (_win.currentStage > 1)

        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        anchors.margins: _win.defaultMargin*(-7);

        Component.onCompleted:
        {
            _subComponent44.startRotationAnimation(600, 360)
        }
    }

}
