import QtQuick 2.13
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.12
import "../singletons"

QQC2.Button {
    Material.background: Material.accent
    Material.foreground: Material.theme === Material.Dark ? Stylesheet.black : Stylesheet.white
}
