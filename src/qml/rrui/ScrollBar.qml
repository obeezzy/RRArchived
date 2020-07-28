import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3

QQC2.ScrollBar {
    size: 0.3
    width: 5

    contentItem: Rectangle {
        color: Material.color(Material.Grey)
        radius: width / 2
    }

}
