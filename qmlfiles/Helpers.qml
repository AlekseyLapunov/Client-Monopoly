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
        Player1,
        Player2,
        Player3,
        Player4,
        Player5,
        Player6,
        NoPlayer
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
        case Helpers.PlayerNumber.NoPlayer:
            return "transparent";
        case Helpers.PlayerNumber.Player1:
            return makeRgb(204, 51, 51);
        case Helpers.PlayerNumber.Player2:
            return makeRgb(204, 153, 51);
        case Helpers.PlayerNumber.Player3:
            return makeRgb(153, 255, 102);
        case Helpers.PlayerNumber.Player4:
            return makeRgb(51, 204, 204);
        case Helpers.PlayerNumber.Player5:
            return makeRgb(51, 51, 204);
        case Helpers.PlayerNumber.Player6:
            return makeRgb(153, 51, 153);
        default:
            return "transparent";
        }
    }

    function defineFieldImageByType(inputFieldType)
    {
        switch(inputFieldType)
        {
        case Helpers.FieldType.Sawmill:
            return "../assets/svgs/fields/businesses/1_sawmill.svg";
        case Helpers.FieldType.CoalStation:
            return "../assets/svgs/fields/businesses/2_coal_station.svg";
        case Helpers.FieldType.AtomicStation:
            return "../assets/svgs/fields/businesses/3_atomic_station.svg";
        case Helpers.FieldType.Forest:
            return "../assets/svgs/fields/resources/1_forest.svg";
        case Helpers.FieldType.Coal:
            return "../assets/svgs/fields/resources/2_coal_source.svg";
        case Helpers.FieldType.Uranium:
            return "../assets/svgs/fields/resources/3_uranium_source.svg";
        case Helpers.FieldType.Vacation:
            return "../assets/svgs/fields/misc/vacation_field.svg";
        case Helpers.FieldType.Sabotage:
            return "../assets/svgs/fields/misc/sabotage_field.svg";
        case Helpers.FieldType.Arrow:
            return "../assets/svgs/fields/misc/arrow_field.svg";
        default:
            return "";
        }
    }

    function defineFieldColorByType(inputFieldType)
    {
        switch(inputFieldType)
        {
        case Helpers.FieldType.Common:
            return makeRgb(245, 255, 250);
        case Helpers.FieldType.Beginning:
            return makeRgb(144, 238, 144);
        case Helpers.FieldType.Sawmill:
            return makeRgb(214, 111, 0);
        case Helpers.FieldType.CoalStation:
            return makeRgb(255, 224, 51);
        case Helpers.FieldType.AtomicStation:
            return makeRgb(0, 230, 0);
        case Helpers.FieldType.Forest:
            return makeRgb(255, 146, 92);
        case Helpers.FieldType.Coal:
            return makeRgb(255, 232, 102);
        case Helpers.FieldType.Uranium:
            return makeRgb(168, 255, 171);
        case Helpers.FieldType.Vacation:
            return makeRgb(179, 102, 255);
        case Helpers.FieldType.Sabotage:
            return makeRgb(155, 45, 48);
        case Helpers.FieldType.Arrow:
            return makeRgb(255, 153, 184);
        default:
            return "transparent";
        }
    }
}
