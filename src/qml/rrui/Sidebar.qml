import QtQuick 2.10
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.2 as QQC2
import Fluid.Controls 1.0 as FluidControls
import "../fontawesome"
import "../models"

FluidControls.Sidebar {
    id: sidebar

    property int currentIndex: 0
    readonly property ListModel model: NavMenuModel { }

    width: 150

    Column {
        id: column
        anchors {
            left: parent.left
            right: parent.right
        }

        Repeater {
            model: sidebar.model
            delegate: QQC2.ItemDelegate {
                id: control
                width: column.width
                height: 40
                highlighted: index === sidebar.currentIndex
                onClicked: sidebar.currentIndex = index;

                contentItem: FocusScope {
                    Row {
                        anchors.verticalCenter: parent.verticalCenter

//                        FluidControls.Icon {
//                            anchors.verticalCenter: parent.verticalCenter
//                            name: iconName
//                        }

                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            text: FontAwesome.fa_android
                            font {
                                pixelSize: 100
                                family: FontAwesome.fontFamily
                            }
                        }

                        FluidControls.CaptionLabel {
                            text: title
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Qt.AlignVCenter
                        }
                    }
                }

                FluidControls.Ripple {
                    anchors.fill: parent
                    control: control
                    //onClicked: control.clicked();
                }
            }
        }
    }
}
