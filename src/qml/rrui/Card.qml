import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.12
import Fluid.Controls 1.0 as FluidControls

FluidControls.Card {
    Material.background: Material.theme === Material.Dark ? Material.color(Material.Grey, Material.Shade800) : "white"
}
