import QtQuick 2.13
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Layouts 1.3 as QQLayouts
import "../../../rrui" as RRUi
import "../../../stock" as Stock
import com.gecko.rr.models 1.0 as RRModels
import QtQuick.Controls.Material 2.3

RRUi.Card {
    id: customerInfo

    property alias name: customerNameField.text
    property alias phoneNumber: customerPhoneNumberField.text

    function clearAll() {
        customerNameField.clear();
        customerPhoneNumberField.clear();
    }

    implicitWidth: 300
    implicitHeight: column.height
    padding: 0
    leftPadding: 4
    rightPadding: 4

    Column {
        id: column
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            topMargin: 0
        }

        QQLayouts.RowLayout {
            spacing: 2
            anchors {
                left: parent.left
                right: parent.right
            }

            FluidControls.Icon {
                QQLayouts.Layout.alignment: Qt.AlignVCenter
                source: FluidControls.Utils.iconUrl("social/person")
                size: 20
                QQLayouts.Layout.preferredWidth: size
                QQLayouts.Layout.preferredHeight: size
            }

            Item { QQLayouts.Layout.preferredWidth: 8; QQLayouts.Layout.fillHeight: true }

            RRUi.TextField {
                id: customerNameField
                focus: true
                QQLayouts.Layout.fillWidth: true
                placeholderText: qsTr("Customer name")
            }

            RRUi.ToolButton {
                id: customerOptionButton
                icon.source: FluidControls.Utils.iconUrl("navigation/more_vert")
                text: qsTr("More")
            }
        }

        Row {
            visible: false
            spacing: 12

            FluidControls.Icon {
                anchors.verticalCenter: parent.verticalCenter
                source: FluidControls.Utils.iconUrl("communication/phone")
                size: 20
            }

            RRUi.TextField {
                id: customerPhoneNumberField
                width: 300
                placeholderText: qsTr("Customer phone number")
            }
        }
    }
}
