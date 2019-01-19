import QtQuick 2.12
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import "../models"

FluidControls.Sidebar {
    id: sidebar

    property int currentIndex: 0
    readonly property ListModel model: NavMenuModel { }

    function findIndexFromFileName(fileName) {
        for (var i = 0; i < sidebar.model.count; ++i) {
            if (fileName === sidebar.model.get(i).fileName)
                return i;
        }

        return -1;
    }

    width: 220

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
                width: sidebar.availableWidth
                height: 40
                highlighted: index === sidebar.currentIndex
                onClicked: sidebar.currentIndex = index;

                contentItem: FocusScope {
                    Row {
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 24

                        FluidControls.Icon {
                            anchors.verticalCenter: parent.verticalCenter
                            source: iconUrl
                        }

                        FluidControls.SubheadingLabel {
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
