import "." as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts

Item {
    id: iconTextField

    property alias icon: icon
    property alias text: textField.text
    property alias textField: textField

    function clear() {
        textField.clear();
    }

    implicitWidth: 300
    implicitHeight: row.height

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
        }

        RRUi.TextField {
            id: textField

            QQLayouts.Layout.fillWidth: true
        }

    }

}
