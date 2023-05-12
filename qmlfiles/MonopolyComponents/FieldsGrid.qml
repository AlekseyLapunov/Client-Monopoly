import QtQuick 2.15
import QtQuick.Controls
import "SubComponents"
import "../HelperSingletone"

import GameMap 1.0

Rectangle
{
    id: root
    property double mapScale: 3.1
    property color sharedColor: Qt.rgba(0, 0, 0, 0)
    property color baseColor: Qt.rgba(sharedColor.r-0.20,
                                      sharedColor.g-0.20,
                                      sharedColor.b-0.20)

    color: "transparent"
    border.color: Qt.darker(baseColor, 1)
    border.width: 3

    signal defineAction(int cellOrderIndex)
    signal setCellsState(string state)
    signal defineStateForSabotage()

    GridView
    {
        id: _grid

        width: root.width - 2*root.border.width
        height: root.height - 2*root.border.width
        anchors.centerIn: root
        interactive: false

        cellHeight: _grid.width/Math.sqrt(_fieldsGridModel.rowCount())
        cellWidth: cellHeight

        model: _fieldsGridModel
        delegate: Cell
        {
            height: _grid.cellHeight
            width: _grid.cellWidth

            state: "normal"

            border.color: root.border.color
            border.width: root.border.width/3

            orderIndex:        model.orderIndex
            fieldType:         model.fieldType
            playerNumberOwner: model.playerNumberOwner
            fieldCost:         model.fieldCost
            fieldIncome:       model.fieldIncome
            piecesOnCellMask:  model.piecesOnCellMask
            stage:             model.stage
            arrowDirection:    model.arrowDirection
            blankUntilStage:   model.blankUntilStage

            cellNumber: index

            Connections
            {
                target: root

                function onDefineAction(cellOrderIndex) { defineActionInternal(cellOrderIndex) }
                function onSetCellsState(state) { setStateInternal(state) }
                function onDefineStateForSabotage() { defineStateForSabotageInternal() }
            }

            PropertyAnimation on opacity
            {
                from: 0.0
                to: 1.0
                duration: 200
            }
        }

        Component.onCompleted:
        {
            _overlayRectOpacityAnimation.duration = 0;
            _overlayRectangle.toggler = true;
        }
    }

    Rectangle
    {
        id: _overlayRectangle
        property bool toggler: false
        color: Qt.lighter(root.baseColor, 1.1)
        anchors.fill: root
        opacity: toggler ? 1.0 : 0.1

        MouseArea
        {
            id: _overlayRectangleClickSilencer
            anchors.fill: _overlayRectangle
            onClicked: {}
            enabled: _overlayRectangle.opacity > 0.2
            visible: enabled
        }

        Behavior on opacity
        {
            PropertyAnimation { id: _overlayRectOpacityAnimation }
        }
    }

    Timer
    {
        id: _overlayRectangleTimer
        interval: _win.stageAnimationDurationMs
        onTriggered:
        {
            _overlayRectOpacityAnimation.duration = 500;
            _overlayRectangle.toggler = false;
        }
    }

    Timer
    {
        id: _resetStatesTimer
        interval: 5000
        repeat: false

        onTriggered:
        {
            root.setCellsState("normal")
        }
    }

    function toggleOverlayRectangleOpacity()
    {
        _overlayRectOpacityAnimation.duration = 0;
        _overlayRectangle.toggler = true;
        _overlayRectangleTimer.start();
    }

    function setOverlayRectangleOpacityZero()
    {
        _overlayRectangle.toggler = false;
    }

    function sabotageActivated()
    {
        defineStateForSabotage();
        _resetStatesTimer.start();
    }

    function resetMapStates()
    {
        setCellsState("normal");
        _resetStatesTimer.stop();
    }
}
