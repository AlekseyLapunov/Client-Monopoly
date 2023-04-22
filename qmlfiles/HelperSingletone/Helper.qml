pragma Singleton
import QtQuick 2.15

Item
{
    enum FieldType
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

    enum PlayerNumber
    {
        NoPlayer,
        Player1,
        Player2,
        Player3,
        Player4,
        Player5,
        Player6
    }

    function makeRgb(r, g, b, alpha = 1)
    {
        const max = 255;
        return Qt.rgba(r/max, g/max, b/max, alpha);
    }

    function definePlayerColorByNumber(playerNumber)
    {
        switch(playerNumber)
        {
        case Helper.PlayerNumber.NoPlayer:
            return "transparent";
        case Helper.PlayerNumber.Player1:
            return makeRgb(204, 51, 51);
        case Helper.PlayerNumber.Player2:
            return makeRgb(204, 153, 51);
        case Helper.PlayerNumber.Player3:
            return makeRgb(153, 255, 102);
        case Helper.PlayerNumber.Player4:
            return makeRgb(51, 204, 204);
        case Helper.PlayerNumber.Player5:
            return makeRgb(51, 51, 204);
        case Helper.PlayerNumber.Player6:
            return makeRgb(153, 51, 153);
        default:
            return "transparent";
        }
    }

    function defineFieldImageByType(inputFieldType)
    {
        switch(inputFieldType)
        {
        case Helper.FieldType.Sawmill:
            return "../../../assets/svgs/fields/businesses/1_sawmill.svg";
        case Helper.FieldType.CoalStation:
            return "../../../assets/svgs/fields/businesses/2_coal_station.svg";
        case Helper.FieldType.AtomicStation:
            return "../../../assets/svgs/fields/businesses/3_atomic_station.svg";
        case Helper.FieldType.Forest:
            return "../../../assets/svgs/fields/resources/1_forest.svg";
        case Helper.FieldType.Coal:
            return "../../../assets/svgs/fields/resources/2_coal_source.svg";
        case Helper.FieldType.Uranium:
            return "../../../assets/svgs/fields/resources/3_uranium_source.svg";
        case Helper.FieldType.Vacation:
            return "../../../assets/svgs/fields/misc/vacation_field.svg";
        case Helper.FieldType.Sabotage:
            return "../../../assets/svgs/fields/misc/sabotage_field.svg";
        case Helper.FieldType.Arrow:
            return "../../../assets/svgs/fields/misc/arrow_field.svg";
        default:
            return "";
        }
    }

    function defineFieldColorByType(inputFieldType)
    {
        switch(inputFieldType)
        {
        case Helper.FieldType.Common:
            return makeRgb(245, 255, 250);
        case Helper.FieldType.Beginning:
            return makeRgb(144, 238, 144);
        case Helper.FieldType.Sawmill:
            return makeRgb(214, 111, 0);
        case Helper.FieldType.CoalStation:
            return makeRgb(255, 224, 51);
        case Helper.FieldType.AtomicStation:
            return makeRgb(0, 230, 0);
        case Helper.FieldType.Forest:
            return makeRgb(255, 146, 92);
        case Helper.FieldType.Coal:
            return makeRgb(255, 232, 102);
        case Helper.FieldType.Uranium:
            return makeRgb(168, 255, 171);
        case Helper.FieldType.Vacation:
            return makeRgb(179, 102, 255);
        case Helper.FieldType.Sabotage:
            return makeRgb(155, 45, 48);
        case Helper.FieldType.Arrow:
            return makeRgb(255, 153, 184);
        default:
            return "transparent";
        }
    }

    function applyContrast(color, factor)
    {
        let red = color.r;
        let green = color.g;
        let blue = color.b;

        let brightness = (red + green + blue)/3;

        red = (red - brightness)*factor + brightness;
        green = (green - brightness)*factor + brightness;
        blue = (blue - brightness)*factor + brightness;

        return Qt.rgba(red, green, blue, color.a);
    }
}