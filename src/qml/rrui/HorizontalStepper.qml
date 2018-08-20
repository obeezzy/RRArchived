import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr 1.0 as RR
import "."

QQC2.Pane {
    id: horizontalStepper

    property alias model: repeater.model
    property int currentIndex: 0
    property real dividerWidth: 84

    implicitWidth: 200
    implicitHeight: 64
    padding: 0

    background: Rectangle { }

    Flickable {
        id: flickable

        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
        }

        leftMargin: 24
        rightMargin: 24
        contentWidth: layout.width
        contentHeight: layout.height
        height: layout.height
        flickableDirection: Flickable.HorizontalFlick
        clip: true

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

                    Item { width: 8; height: 1 } // Spacer

                    FluidControls.TitleLabel {
                        text: modelData
                        font.bold: horizontalStepper.currentIndex === index
                    }

                    Item { width: 8; height: 1 } // Spacer

                    Rectangle {
                        anchors.verticalCenter: parent.verticalCenter
                        width: horizontalStepper.dividerWidth
                        height: 1
                        color: Material.dividerColor
                        visible: index !== repeater.count - 1
                    }

                    Item { width: 8; height: 1 } // Spacer
                }
            }
        }
    }

    FluidControls.ThinDivider { anchors.bottom: parent.bottom }
}
