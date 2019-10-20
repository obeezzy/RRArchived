import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3

QQC2.Popup {
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    parent: MainWindow.contentItem
    dim: true
    modal: true
    implicitWidth: 640
    implicitHeight: 640
}
