import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "." as RRUi

Item {
    id: iconTextField
    implicitWidth: 300
    implicitHeight: row.height

    property alias icon: icon
    property alias textField: textField

    QQLayouts.RowLayout {
        id: row
        spacing: 12
        anchors {
            left: parent.left
            right: parent.right
        }

        FluidControls.Icon {
            id: icon
            QQLayouts.Layout.alignment: Qt.AlignVCenter
            name: "action/bug_report"
        }

        RRUi.TextField {
            id: textField
            QQLayouts.Layout.fillWidth: true
        }
    }
}
