import QtQuick 2.15
import QtQuick3D
import QtQuick3D.Helpers

Node
{
    position: Qt.vector3d(0, 0, 0)
    property color diceColor: "white"

    PrincipledMaterial
    {
        id: _priDiceMaterial
        baseColorMap: Texture
        {
            source: "../assets/3d_dice/Dice_Diffuse.png"
        }
        baseColor: diceColor
    }

    Model
    {
       id: _firstDiceModel
       source: "../assets/3d_dice/dice_001.mesh"
       materials: [_priDiceMaterial]
       z: -10
    }
}
