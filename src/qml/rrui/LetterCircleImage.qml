import QtQuick 2.10
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls

FluidControls.CircleImage {
    id: letterCircleImage

    property string name: ""
    property alias font: label.font
    property int maximumLetterCount: 2
    readonly property var selectableColors: [
        Material.color(Material.Red),
        Material.color(Material.Pink),
        Material.color(Material.Purple),
        Material.color(Material.DeepPurple),
        Material.color(Material.Indigo),
        Material.color(Material.Blue),
        Material.color(Material.LightBlue),
        Material.color(Material.Cyan),
        Material.color(Material.Teal),
        Material.color(Material.LightGreen),
        Material.color(Material.Lime),
        Material.color(Material.Amber),
        Material.color(Material.Orange),
        Material.color(Material.DeepOrange),
        Material.color(Material.Brown),
        Material.color(Material.Grey),
        Material.color(Material.BlueGrey)
    ]

    width: 32
    height: 32
    smooth: true

    Rectangle {
        color: selectableColors[Math.floor(Math.random() * selectableColors.length)]
        anchors.fill: parent
        radius: letterCircleImage.width / 2
        smooth: true
        //visible: letterCircleImage.source === ""

        FluidControls.SubheadingLabel {
            id: label
            anchors.centerIn: parent
            color: "white"
            font.capitalization: Font.AllUppercase
            text: {
                var words = letterCircleImage.name.split(" ");
                var letters = [];

                for (var i = 0; i < words.length; ++i) {
                    if (letters.length < letterCircleImage.maximumLetterCount)
                        letters.push(words[i][0]);
                }

                return letters.join("");
            }
        }
    }
}
