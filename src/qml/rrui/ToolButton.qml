import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3

FluidControls.ToolButton {
    QQC2.ToolTip.visible: hovered && text !== ""
    QQC2.ToolTip.delay: 300
    QQC2.ToolTip.text: text
    display: QQC2.AbstractButton.IconOnly
}
