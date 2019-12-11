import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Controls.Material 2.3
import "../../../rrui" as RRUi
import "../../../singletons"

RRUi.Card {
    id: checkout

    property real totalCost: 0
    signal checkoutRequested

    implicitWidth: 300
    implicitHeight: column.height
    padding: 2
    bottomPadding: 0

    Column {
        id: column
        anchors {
            left: parent.left
            right: parent.right
        }

        FluidControls.SubheadingLabel {
            anchors.right: parent.right
            text: qsTr("Total cost: %1").arg(Number(checkout.totalCost)
                                             .toLocaleCurrencyString(Qt.locale(GlobalSettings.localeName)))
        }

        Row {
            anchors.right: parent.right
            spacing: 8

            QQC2.Button { text: qsTr("View totals") }

            RRUi.PrimaryButton {
                text: qsTr("Proceed to Checkout")
                onClicked: checkout.checkoutRequested();
            }
        }
    }
}
