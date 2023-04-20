import QtQuick 2.15
import Qt5Compat.GraphicalEffects
import "../../HelperSingletone"

Image
{
    id: root

    property int whatPlayer: Helper.PlayerNumber.NoPlayer

    source: (whatPlayer === Helper.PlayerNumber.NoPlayer) ? "" :
                                                             "../../../assets/svgs/player/gaming_piece.svg"
    visible: (whatPlayer !== Helper.PlayerNumber.NoPlayer)

    ColorOverlay
    {
        source: root
        color: Helper.definePlayerColorByNumber(root.whatPlayer)
    }
}
