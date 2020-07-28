import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.13
import QtQuick.Controls 2.13 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts

QQC2.Dialog {
    id: dialog

    implicitWidth: 300
    implicitHeight: 400
    anchors.centerIn: QQC2.Overlay.overlay
    parent: QQC2.Overlay.overlay
    focus: visible
    modal: true
    title: qsTr("Title")
    standardButtons: QQC2.Dialog.Ok | QQC2.Dialog.Cancel
}
