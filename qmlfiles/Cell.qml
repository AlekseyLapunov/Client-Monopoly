import QtQuick 2.15
import Qt5Compat.GraphicalEffects

Rectangle
{
    id: root

    enum Type
    {
        Void,
        Common,
        Beginning,
        Sawmill,
        CoalStation,
        AtomicStation,
        Forest,
        Coal,
        Uranium,
        Vacation,
        Sabotage,
        Arrow
    }

    property string backgroundColor: "transparent"
    property int fieldType: Cell.Type.Void

    border.color: "black"
    border.width: 2
    visible: fieldType === Cell.Type.Void ? false : true
    color: defineColorByType(fieldType)

    Image
    {
        id: _ownerFrame
        anchors.fill: root
    }

    Image
    {
        id: _fieldTypeImage
        anchors.centerIn: root
        source: defineImageByType(fieldType)
        smooth: true
        sourceSize.width: root.width*0.9
        sourceSize.height: root.height*0.9
    }

    Desaturate
    {
        anchors.fill: _fieldTypeImage
        source: _fieldTypeImage
    }

    GamingPiecesDisplay
    {
        id: _gamingPiecesDisplay
        anchors.fill: root
    }

    function defineImageByType(inputFieldType)
    {
        switch(inputFieldType)
        {
        case Cell.Type.Sawmill:
            return "../assets/svgs/fields/businesses/1_sawmill.svg";
        case Cell.Type.CoalStation:
            return "../assets/svgs/fields/businesses/2_coal_station.svg";
        case Cell.Type.AtomicStation:
            return "../assets/svgs/fields/businesses/3_atomic_station.svg";
        case Cell.Type.Forest:
            return "../assets/svgs/fields/resources/1_forest.svg";
        case Cell.Type.Coal:
            return "../assets/svgs/fields/resources/2_coal_source.svg";
        case Cell.Type.Uranium:
            return "../assets/svgs/fields/resources/3_uranium_source.svg";
        case Cell.Type.Vacation:
            return "../assets/svgs/fields/misc/vacation_field.svg";
        case Cell.Type.Sabotage:
            return "../assets/svgs/fields/misc/sabotage_field.svg";
        case Cell.Type.Arrow:
            return "../assets/svgs/fields/misc/arrow_field.svg";
        default:
            return "";
        }
    }

    function defineColorByType(inputFieldType)
    {
        switch(inputFieldType)
        {
        case Cell.Type.Common:
            return _displayField.makeRgb(245, 255, 250);
        case Cell.Type.Beginning:
            return _displayField.makeRgb(144, 238, 144);
        case Cell.Type.Sawmill:
            return _displayField.makeRgb(214, 111, 0);
        case Cell.Type.CoalStation:
            return _displayField.makeRgb(255, 224, 51);
        case Cell.Type.AtomicStation:
            return _displayField.makeRgb(0, 230, 0);
        case Cell.Type.Forest:
            return _displayField.makeRgb(255, 146, 92);
        case Cell.Type.Coal:
            return _displayField.makeRgb(255, 232, 102);
        case Cell.Type.Uranium:
            return _displayField.makeRgb(77, 255, 77);
        case Cell.Type.Vacation:
            return _displayField.makeRgb(179, 102, 255);
        case Cell.Type.Sabotage:
            return _displayField.makeRgb(155, 45, 48);
        case Cell.Type.Arrow:
            return _displayField.makeRgb(255, 153, 184);
        default:
            return "transparent";
        }
    }
}
