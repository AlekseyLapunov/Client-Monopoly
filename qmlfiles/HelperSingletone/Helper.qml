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

    enum ArrowRotation
    {
        ArrowUp,
        ArrowRight,
        ArrowDown,
        ArrowLeft
    }

    enum PlayerModelRole
    {
        PlayerNumberRole = 256,
        DisplayableNameRole,
        CurrentBalanceRole,
        PiecePositionOnOrderIndexRole
    }

    function makeRgb(r, g, b, alpha = 1)
    {
        const max = 255;
        return Qt.rgba(r/max, g/max, b/max, alpha);
    }

    function definePlayerNumberBeforeHost(inputHostPlayerNumber, inputPlayersCount)
    {
        let playerNumberBeforeHost = inputHostPlayerNumber - 1;

        if(playerNumberBeforeHost === Helper.PlayerNumber.NoPlayer)
            return inputPlayersCount;
        else return playerNumberBeforeHost;
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

    function defineFieldNameByType(inputFieldType)
    {
        switch(inputFieldType)
        {
        case Helper.FieldType.Beginning:
            return "Стартовое поле";
        case Helper.FieldType.Sawmill:
            return "Лесопилка";
        case Helper.FieldType.CoalStation:
            return "Тепловая электростанция";
        case Helper.FieldType.AtomicStation:
            return "Атомная электростанция";
        case Helper.FieldType.Forest:
            return "Лесной массив";
        case Helper.FieldType.Coal:
            return "Залежи угля";
        case Helper.FieldType.Uranium:
            return "Залежи урана";
        case Helper.FieldType.Vacation:
            return "Отпуск";
        case Helper.FieldType.Sabotage:
            return "Диверсия";
        case Helper.FieldType.Arrow:
            return "Стрелка";
        default:
            return "";
        }
    }

    function defineFieldDescriptionByType(inputFieldType)
    {
        switch(inputFieldType)
        {
        case Helper.FieldType.Beginning:
            return "Награждает игрока единовременной выплатой ЭК при повторном попадании его фишки.";
        case Helper.FieldType.Sawmill:
            return "Приносит доход за счёт продажи обработанной древесины.";
        case Helper.FieldType.CoalStation:
            return "Приносит доход за счёт производства энергии, получаемой при обработке ископаемого угля.";
        case Helper.FieldType.AtomicStation:
            return "Приносит доход за счёт производства энергии, получаемой при распаде урана.";
        case Helper.FieldType.Forest:
            return "Увеличивает доходность подконтрольных лесопилок, а также позволяет получать часть прибыли от лесопилок, которыми владеют другие игроки.";
        case Helper.FieldType.Coal:
            return "Увеличивают доходность подконтрольных тепловых электростанций, а также позволяют получать часть прибыли от ТЭС, которыми владеют другие игроки.";
        case Helper.FieldType.Uranium:
            return "Увеличивают доходность подконтрольных атомных электростанций, а также позволяют получать часть прибыли от АЭС, которыми владеют другие игроки.";
        case Helper.FieldType.Vacation:
            return "Вынуждает отправиться в отпуск. За это полагается бонус в виде единовременной выплаты ЭК!";
        case Helper.FieldType.Sabotage:
            return "Позволяет провести диверсию по отношению к другому игроку. Выберите актив, который потеряет контроль, а его владелец дополнительно лишится процентов от своего текущего баланса.";
        case Helper.FieldType.Arrow:
            return "Направляет фишку игрока в ту сторону, в которую повёрнута. Игрок, прошедший через данное поле, может повернуть стрелку в любую другую доступную сторону за определённую плату. В противном случае указатель повернётся по часовой стрелке в ближайшее разрешённое направление.";
        default:
            return "";
        }
    }

    function defineRotationByFieldType(inputFieldType: int, arrowRotation: int)
    {
        if(inputFieldType !== Helper.FieldType.Arrow)
            return 0;

        switch(arrowRotation)
        {
        case Helper.ArrowRotation.ArrowUp:
            return 0;
        case Helper.ArrowRotation.ArrowRight:
            return 90;
        case Helper.ArrowRotation.ArrowDown:
            return 180;
        case Helper.ArrowRotation.ArrowLeft:
            return 270;
        default:
            return 0;
        }
    }

    function canFieldExecuteDialog(inputFieldType: int)
    {
        return (inputFieldType >= Helper.FieldType.Beginning
                && inputFieldType <= Helper.FieldType.Arrow);
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

    function definePiecesLeftMask(mask1: int, mask2: int)
    {
      let piecesLeftMask = 0;
      let bit = 1;
      while (mask1 > 0)
      {
        if ((mask1 & 1) === 1 && (mask2 & 1) === 0)
        {
          piecesLeftMask |= bit;
        }
        mask1 >>= 1;
        mask2 >>= 1;
        bit <<= 1;
      }
      return piecesLeftMask;
    }

    function definePiecesAppearMask(mask1: int, mask2: int)
    {
      const diff = mask1 ^ mask2;
      let mask = 0;
      for (let i = 0; i < (Helper.PlayerNumber.Player6 - 1); i++)
      {
        if ((diff >> i) & 1)
        {
          if ((mask2 >> i) & 1)
          {
            mask |= (1 << i);
          }
        }
      }
      return mask;
    }
}
