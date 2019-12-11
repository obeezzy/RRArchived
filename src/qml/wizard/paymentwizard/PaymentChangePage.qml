import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr 1.0 as RR
import "../../rrui" as RRUi
import "../../singletons"
import ".."

RRUi.WizardPage {
    id: paymentChangePage
    objectName: "paymentChangePage"

    property real changeDue: 0

    padding: FluidControls.Units.smallSpacing
    title: qsTr("Change due")
    hasNext: true

    FluidControls.HeadlineLabel {
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
        }
        wrapMode: Text.WordWrap
        horizontalAlignment: Qt.AlignHCenter
        text: qsTr("Please hand over %1 to the customer.").arg(Number(Math.abs(paymentChangePage.changeDue))
                                                               .toLocaleCurrencyString(Qt.locale(GlobalSettings.localeName)))
    }

    QQC2.CheckBox {
        id: printCheckBox
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
        text: qsTr("Print receipt")
    }

    onNext: {
        if (printCheckBox.checked)
            receiptPrinter.print(cartModel.toPrintableFormat());
        paymentChangePage.nextPage.component = Qt.resolvedUrl("PaymentFinishPage.qml");
        paymentChangePage.finish();
    }

    RR.ReceiptPrinter { id: receiptPrinter }
}
