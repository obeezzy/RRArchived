import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Layouts 1.3 as QQLayouts

QQC2.Dialog {
    id: dialog

    implicitWidth: 300
    implicitHeight: 400
    x: (QQC2.ApplicationWindow.contentItem.width - width) / 2
    y: (QQC2.ApplicationWindow.contentItem.height - height) / 2
    parent: QQC2.ApplicationWindow.contentItem
    focus: visible
    modal: true
    title: qsTr("Title")
    standardButtons: QQC2.Dialog.Ok | QQC2.Dialog.Cancel
}
