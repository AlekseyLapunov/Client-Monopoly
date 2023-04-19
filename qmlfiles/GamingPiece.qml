import QtQuick 2.15
import Qt5Compat.GraphicalEffects

Image
{
    id: root

    property int whatPlayer: Helpers.PlayerNumber.NoPlayer

    source: (whatPlayer === Helpers.PlayerNumber.NoPlayer) ? "" :
                                                             "../assets/svgs/player/gaming_piece.svg"
    visible: (whatPlayer !== Helpers.PlayerNumber.NoPlayer)

    ColorOverlay
    {
        source: root
        color: Helpers.definePlayerColorByNumber(root.whatPlayer)
    }
}
