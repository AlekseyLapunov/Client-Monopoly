import QtQuick 2.15
import Qt5Compat.GraphicalEffects

Rectangle
{
    id: root

    property string backgroundColor: "transparent"
    property int fieldType: Helpers.FieldType.Void
    //property list playersOnIt: []

    visible: fieldType === Helpers.FieldType.Void ? false : true
    color: Helpers.defineFieldColorByType(fieldType)

    Image
    {
        id: _ownerFrame
        anchors.fill: root
    }

    Image
    {
        id: _fieldTypeImage
        anchors.centerIn: root
        source: Helpers.defineFieldImageByType(fieldType)
        smooth: true
        sourceSize.width: root.width*0.9
        sourceSize.height: root.height*0.9
    }

    GamingPiecesDisplay
    {
        id: _gamingPiecesDisplay
        anchors.fill: root
    }
}
