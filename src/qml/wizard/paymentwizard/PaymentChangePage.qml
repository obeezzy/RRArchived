import ".."
import "../../rrui" as RRUi
import "../../singletons"
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import com.gecko.rr 1.0 as RR

RRUi.WizardPage {
    id: paymentChangePage

    property real changeDue: 0

    objectName: "paymentChangePage"
    padding: FluidControls.Units.smallSpacing
    title: qsTr("Change due")
    hasNext: true
    onNext: {
        if (printCheckBox.checked)
            receiptPrinter.print(cartModel.toPrintableFormat());

        paymentChangePage.nextPage.component = Qt.resolvedUrl("PaymentFinishPage.qml");
        paymentChangePage.finish();
    }

    FluidControls.HeadlineLabel {
        wrapMode: Text.WordWrap
        horizontalAlignment: Qt.AlignHCenter
        text: qsTr("Please hand over %1 to the customer.").arg(Number(Math.abs(paymentChangePage.changeDue)).toLocaleCurrencyString(Qt.locale(GlobalSettings.localeName)))

        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
        }

    }

    QQC2.CheckBox {
        id: printCheckBox

        text: qsTr("Print receipt")

        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

    }

    RR.ReceiptPrinter {
        id: receiptPrinter
    }

}
