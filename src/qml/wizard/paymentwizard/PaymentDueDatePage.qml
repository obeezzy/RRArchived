import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.12
import Fluid.Controls 1.1 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../../rrui" as RRUi
import "../../singletons"
import ".."

RRUi.WizardPage {
    id: paymentDueDatePage
    objectName: "paymentDueDatePage"

    property date dueDate: new Date()
    property RRModels.SaleCartModel cartModel: null

    padding: FluidControls.Units.smallSpacing
    title: qsTr("Choose due date")
    hasNext: true

    Column {
        padding: FluidControls.Units.smallSpacing

        anchors {
            left: parent.left
            right: parent.right
        }

        FluidControls.SubheadingLabel {
            anchors {
                left: parent.left
                right: parent.right
            }

            text: qsTr("Choose a date and time that you want the customer to pay.")
        }

        FluidControls.DateTimePicker {
            prefer24Hour: false
            selectedDateTime: {
                let date = new Date();
                date.setDate(date.getDate() + GlobalSettings._DAYS_IN_A_WEEK); // A week later
                return date;
            }
        }
    }

    onNext: {
        paymentDueDatePage.QQC2.StackView.view.wizard.dueDate = paymentDueDatePage.dueDate;
        paymentDueDatePage.nextPage.component = Qt.resolvedUrl("PaymentSummaryPage.qml");
        paymentDueDatePage.nextPage.properties = {
            "customerName": paymentDueDatePage.cartModel.customerName,
            "customerPhoneNumber": paymentDueDatePage.cartModel.customerPhoneNumber,
            "totalCost": paymentDueDatePage.cartModel.totalCost,
            "amountPaid": paymentDueDatePage.cartModel.amountPaid,
            "paymentModel": paymentDueDatePage.cartModel.paymentModel,
            "cartModel": paymentDueDatePage.cartModel
        };
    }
}
