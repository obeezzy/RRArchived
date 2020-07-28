import "../../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls.Material 2.3
import QtQuick.Dialogs 1.3 as Dialogs

Item {
    id: productImage

    readonly property bool dirty: privateProperties.dirty
    property alias name: image.name
    property alias source: image.source

    implicitWidth: 160
    implicitHeight: column.height

    QtObject {
        id: privateProperties

        property bool dirty: false
    }

    Column {
        id: column

        spacing: 4

        anchors {
            left: parent.left
            right: parent.right
        }

        RRUi.LetterCircleImage {
            id: image

            height: width
            font.pixelSize: 30
            sourceSize: Qt.size(width, height)
            onSourceChanged: privateProperties.dirty = true

            anchors {
                left: parent.left
                right: parent.right
            }

            FluidControls.Icon {
                anchors.centerIn: parent
                visible: image.name === "" && image.status !== Image.Ready
                color: Material.theme === Material.Dark ? Material.color(Material.Grey, Material.Shade800) : "white"
                source: FluidControls.Utils.iconUrl("image/photo_camera")
            }

        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter

            RRUi.ToolButton {
                id: takePhotoButton

                icon.source: FluidControls.Utils.iconUrl("image/photo_camera")
                text: qsTr("Take a photo")
            }

            RRUi.ToolButton {
                id: selectPhotoButton

                icon.source: FluidControls.Utils.iconUrl("image/photo")
                text: qsTr("Select image")
                onClicked: fileDialog.visible = true
            }

            RRUi.ToolButton {
                id: deviceSearchButton

                icon.source: FluidControls.Utils.iconUrl("hardware/phonelink")
                text: qsTr("Start DeviceLink")
            }

        }

    }

    Dialogs.FileDialog {
        id: fileDialog

        title: qsTr("Please choose a file")
        folder: shortcuts.home
        nameFilters: ["Image files (*.jpg *.png *.jpeg)"]
        onAccepted: productImage.source = fileDialog.fileUrl
    }

}
