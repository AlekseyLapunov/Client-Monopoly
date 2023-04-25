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

    enum Dice
    {
        Left,
        Right
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
            return makeRgb(135, 206, 250);
        case Helper.FieldType.Sawmill:
            return makeRgb(255, 142, 13);
        case Helper.FieldType.CoalStation:
            return makeRgb(255, 224, 51);
        case Helper.FieldType.AtomicStation:
            return makeRgb(77, 255, 77);
        case Helper.FieldType.Forest:
            return makeRgb(255, 167, 66);
        case Helper.FieldType.Coal:
            return makeRgb(255, 232, 102);
        case Helper.FieldType.Uranium:
            return makeRgb(128, 255, 128);
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

    function fontWarningPreventor(pointSize)
    {
        return pointSize === 0 ? 1 : pointSize;
    }

    function getRandomInt(includedMin, includedMax)
    {
        let value = Math.floor((includedMin + Math.random()*((includedMax+1)-includedMin)));
        return value;
    }

    function getRandomQtVector(includedMin, includedMax)
    {
        return Qt.vector3d(getRandomInt(includedMin, includedMax),
                           getRandomInt(includedMin, includedMax),
                           getRandomInt(includedMin, includedMax));
    }

    function getVectorDiff(vec1: vector3d, vec2: vector3d)
    {
        let sum1 = vec1.x + vec1.y + vec1.z;
        let sum2 = vec2.x + vec2.y + vec2.z;
        return Math.abs(sum2 - sum1);
    }

    function getDirectionalRotate(diceNumber: int, rightDiceCorrection: int)
    {
        let correction = (rightDiceCorrection === Helper.Dice.Right) ? -22 : 0;
        switch(diceNumber)
        {
        case 1:
            return Qt.vector3d(0, 11 + correction, 0);
        case 2:
            return Qt.vector3d(0, -76 + correction*1.25, 0);
        case 3:
            return Qt.vector3d(90, 11 + correction, 0);
        case 4:
            return Qt.vector3d(270, 11 + correction, 0);
        case 5:
            return Qt.vector3d(90, 105 + correction*1.25, 0);
        case 6:
            return Qt.vector3d(0, 195 + correction*1.4, 90);
        default: return;
        }
    }
}
