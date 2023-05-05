import QtQuick 2.15
import Qt5Compat.GraphicalEffects
import "../../HelperSingletone"

Rectangle
{
    id: root

    property double lightingCoeff: 1.05
    property color localShareGradColor1
    property color localShareGradColor2
    property int curStageNumber: _win.currentStage
    property int curTurnNumber: 1

    color: "transparent"

    Text
    {
        id: _currentStageLabel
        anchors.right: _separator.left
        anchors.verticalCenter: root.verticalCenter
        color: Helper.applyContrast(localShareGradColor1, 1.5)
        font.family: "Bookman Old Style"
        font.pointSize: Helper.fontWarningPreventor(root.width*0.048)
        font.bold: true
        horizontalAlignment: Text.AlignRight
        style: Text.Outline
        styleColor: Qt.darker(Qt.rgba(_currentStageLabel.color.r,
                                      _currentStageLabel.color.g,
                                      _currentStageLabel.color.b,
                                      0.5).toString(), 1.5)
        text: ("Стадия: " + curStageNumber.toString())
    }

    Text
    {
        id: _turnsPastLabel
        anchors.left: _separator.right
        anchors.leftMargin: _win.defaultMargin
        anchors.verticalCenter: root.verticalCenter
        color: Helper.applyContrast(localShareGradColor2, 1.5)
        font.family: "Bookman Old Style"
        font.pointSize: Helper.fontWarningPreventor(root.width*0.048)
        font.bold: true
        horizontalAlignment: Text.AlignLeft
        style: Text.Outline
        styleColor: Qt.darker(Qt.rgba(_turnsPastLabel.color.r,
                                      _turnsPastLabel.color.g,
                                      _turnsPastLabel.color.b,
                                      0.5).toString(), 1.5)
        text: ("Ход: " + curTurnNumber.toString())
    }

    Rectangle
    {
        id: _separator
        anchors.centerIn: root
        height: root.height
        width: 2
        visible: false
    }

    LinearGradient
    {
        id: _linearGradient
        anchors.fill: root
        start: Qt.point(root.width, 0)
        end: Qt.point(0, root.height)
        z: -1
        gradient: Gradient
        {
            GradientStop { id: _firstGrad
                           position: 0.0;
                           color: _displayField.backgroundColor1
                           PropertyAnimation on color
                           {
                               id: _backgroundAnimation1
                               from: Qt.lighter(_displayField.backgroundColor1, lightingCoeff);
                               to: Qt.lighter(_displayField.backgroundColor2, lightingCoeff);
                               duration: _displayField.backgroundAnimationDuration
                               onFinished:
                               {
                                   let changer;
                                   changer = _backgroundAnimation1.from.toString();
                                   _backgroundAnimation1.from = _backgroundAnimation1.to.toString();
                                   _backgroundAnimation1.to = changer.toString();
                                   _backgroundAnimation1.restart();
                               }
                           }
                         }
            GradientStop { id: _secondGrad
                           position: 1.0;
                           color: _displayField.backgroundColor2
                           PropertyAnimation on color
                           {
                               id: _backgroundAnimation2
                               from: Qt.lighter(_displayField.backgroundColor2, lightingCoeff);
                               to: Qt.lighter(_displayField.backgroundColor1, lightingCoeff);
                               duration: _displayField.backgroundAnimationDuration
                               onFinished:
                               {
                                   let changer;
                                   changer = _backgroundAnimation2.from.toString();
                                   _backgroundAnimation2.from = _backgroundAnimation2.to.toString();
                                   _backgroundAnimation2.to = changer.toString();
                                   _backgroundAnimation2.restart();
                               }
                           }
                         }
        }
    }

    PropertyAnimation on localShareGradColor1
    {
        id: _shareGradAnimation1
        from: _displayField.backgroundColor2
        to: _displayField.backgroundColor1
        duration: _displayField.backgroundAnimationDuration
        onFinished:
        {
            let changer;
            changer = _shareGradAnimation1.from.toString();
            _shareGradAnimation1.from = _shareGradAnimation1.to.toString();
            _shareGradAnimation1.to = changer.toString();
            _shareGradAnimation1.restart();
        }
    }

    PropertyAnimation on localShareGradColor2
    {
        id: _shareGradAnimation2
        from: _displayField.backgroundColor1
        to: _displayField.backgroundColor2
        duration: _displayField.backgroundAnimationDuration
        onFinished:
        {
            let changer;
            changer = _shareGradAnimation2.from.toString();
            _shareGradAnimation2.from = _shareGradAnimation2.to.toString();
            _shareGradAnimation2.to = changer.toString();
            _shareGradAnimation2.restart();
        }
    }
}
