import "."
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import com.gecko.rr 1.0 as RR

QQC2.Pane {
    id: horizontalStepper

    property alias model: repeater.model
    property int currentIndex: 0
    property real dividerWidth: 84

    implicitWidth: 200
    implicitHeight: 64
    padding: 0

    Flickable {
        id: flickable

        leftMargin: 24
        rightMargin: 24
        contentWidth: layout.width
        contentHeight: layout.height
        height: layout.height
        flickableDirection: Flickable.HorizontalFlick
        clip: true

        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
        }

        Row {
            id: layout

            Repeater {
                id: repeater

                delegate: Row {
                    id: row

                    anchors.verticalCenter: parent.verticalCenter

                    Rectangle {
                        width: 30
                        height: width
                        radius: width / 2
                        color: Material.color(Material.Blue)

                        FluidControls.Icon {
                            anchors.centerIn: parent
                            color: "white"
                            source: FluidControls.Utils.iconUrl("navigation/check")
                            visible: horizontalStepper.currentIndex > index
                        }

                        FluidControls.SubheadingLabel {
                            anchors.centerIn: parent
                            text: index + 1
                            color: "white"
                            visible: horizontalStepper.currentIndex <= index
                        }

                    }

                    // Spacer
                    Item {
                        width: 8
                        height: 1
                    }

                    FluidControls.TitleLabel {
                        text: modelData
                        font.bold: horizontalStepper.currentIndex === index
                    }

                    // Spacer
                    Item {
                        width: 8
                        height: 1
                    }

                    Rectangle {
                        anchors.verticalCenter: parent.verticalCenter
                        width: horizontalStepper.dividerWidth
                        height: 1
                        color: Material.dividerColor
                        visible: index !== repeater.count - 1
                    }

                    // Spacer
                    Item {
                        width: 8
                        height: 1
                    }

                }

            }

        }

    }

    FluidControls.ThinDivider {
        anchors.bottom: parent.bottom
    }

    background: Rectangle {
        color: Material.theme === Material.Dark ? Material.color(Material.Grey, Material.Shade800) : "white"
    }

}
