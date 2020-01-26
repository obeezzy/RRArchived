import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../../rrui" as RRUi
import ".."

RRUi.WizardPage {
    id: paymentByCardPage
    objectName: "paymentAmountPage"

    property RRModels.SaleCartModel cartModel: null
    property real totalCost: 0
    //property real amountPaid: parseFloat(amountPaidField.text == "" ? 0 : amountPaidField.text)
    readonly property real balance: 0 //totalCost - amountPaid
    readonly property bool hasDebt: balance > 0
    readonly property bool hasCredit: balance < 0

    padding: FluidControls.Units.smallSpacing
    title: qsTr("Enter card details")
    hasNext: true

    onNext: {
        paymentByCardPage.cartModel.addPayment(paymentByCardPage.amountPaid,
                                               paymentByCardPage.paymentMethod === "debit_card"
                                               ? RRModels.SaleCartModel.DebitCard : RRModels.SaleCartModel.CreditCard);

        if (paymentByCardPage.hasDebt || paymentByCardPage.hasCredit) {
            paymentByCardPage.nextPage.component = Qt.resolvedUrl("PaymentBalancePage.qml");
            paymentByCardPage.nextPage.properties = {
                "hasDebt": paymentByCardPage.hasDebt,
                "hasCredit": paymentByCardPage.hasCredit,
                "balance": paymentByCardPage.cartModel.balance,
                "canAcceptAlternatePaymentMethod": paymentByCardPage.cartModel.canAcceptCash
                                                   || paymentByCardPage.canAcceptCard,
                "isCashPayment": false,
                "totalCost": paymentByCardPage.cartModel.totalCost,
                "cartModel": paymentByCardPage.cartModel
            };
        } else if (paymentByCardPage.cartModel.customerName.trim() === ""
                   || paymentByCardPage.cartModel.customerPhoneNumber.trim() === "") {
            paymentByCardPage.nextPage.component = Qt.resolvedUrl("PaymentCustomerDetailPage.qml");
            paymentByCardPage.nextPage.properties = {
                "customerName": paymentByCardPage.cartModel.customerName,
                "customerPhoneNumber": paymentByCardPage.cartModel.customerPhoneNumber,
                "paymentModel": paymentByCardPage.cartModel.paymentModel,
                "cartModel": paymentByCardPage.cartModel
            };
        } else {
            paymentByCardPage.nextPage.component = Qt.resolvedUrl("PaymentSummaryPage.qml");
            paymentByCardPage.nextPage.properties = {
                "customerName": paymentByCardPage.cartModel.customerName,
                "customerPhoneNumber": paymentByCardPage.cartModel.customerPhoneNumber,
                "totalCost": paymentByCardPage.cartModel.totalCost,
                "amountPaid": paymentByCardPage.cartModel.amountPaid,
                "paymentModel": paymentByCardPage.cartModel.paymentModel,
                "cartModel": paymentByCardPage.cartModel
            };
        }
    }
}
