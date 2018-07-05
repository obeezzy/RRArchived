import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import Fluid.Controls 1.0 as FluidControls
import "../../singletons"

QQC2.Page {
    id: paymentChangePage
    objectName: "paymentChangePage"

    property real changeDue: 0

    padding: FluidControls.Units.smallSpacing
    title: qsTr("Change due")

    FluidControls.HeadlineLabel {
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
        }
        wrapMode: Text.WordWrap
        horizontalAlignment: Qt.AlignHCenter
        text: qsTr("Please hand over %1 to the customer.").arg(Number(Math.abs(paymentChangePage.changeDue)).toLocaleCurrencyString(Qt.locale(GlobalSettings.localeName)))
    }
}
