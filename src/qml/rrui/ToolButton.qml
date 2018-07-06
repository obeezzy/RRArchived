import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls

FluidControls.ToolButton {
    QQC2.ToolTip.visible: hovered && text !== ""
    QQC2.ToolTip.delay: 300
    QQC2.ToolTip.text: text
    display: QQC2.AbstractButton.IconOnly
}
