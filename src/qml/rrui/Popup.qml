import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls.Material 2.3

QQC2.Popup {
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    parent: QQC2.ApplicationWindow.contentItem
    dim: true
    modal: true
    implicitWidth: 640
    implicitHeight: 640
}
