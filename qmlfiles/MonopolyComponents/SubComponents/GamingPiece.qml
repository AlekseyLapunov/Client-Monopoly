import QtQuick 2.15
import Qt5Compat.GraphicalEffects
import "../../HelperSingletone"

Image
{
    id: root

    property int whatPlayer: Helper.PlayerNumber.NoPlayer
    property color colorOverlay: Helper.definePlayerColorByNumber(root.whatPlayer)

    source: (whatPlayer === Helper.PlayerNumber.NoPlayer) ? "" :
                                                             "../../../assets/svgs/player/gaming_piece.svg"
    visible: (whatPlayer !== Helper.PlayerNumber.NoPlayer)

    ColorOverlay
    {
        source: root
        anchors.fill: root
        color: colorOverlay
    }
}
