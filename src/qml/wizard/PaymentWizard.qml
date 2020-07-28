import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import com.gecko.rr.models 1.0 as RRModels
import "paymentwizard"

RRUi.Wizard {
    id: paymentWizard

    enum Reason {
        Sales,
        Purchase
    }

    property RRModels.SaleCartModel cartModel: null
    property string action: ""
    property date dueDate: new Date()
    property int reason: PaymentWizard.Sales

    onClosed: {
        privateProperties.resetCustomerDetails();
    }

    QtObject {
        id: privateProperties

        property string previouslySetCustomerName: ""
        property string previouslySetCustomerPhoneNumber: ""

        function resetCustomerDetails() {
            cartModel.customerName = privateProperties.previouslySetCustomerName;
            cartModel.customerPhoneNumber = privateProperties.previouslySetCustomerPhoneNumber;
        }

    }

    initialPage: PaymentMethodPage {
        canAcceptCash: paymentWizard.cartModel.canAcceptCash
        canAcceptCard: paymentWizard.cartModel.canAcceptCard
        cartModel: paymentWizard.cartModel
        reason: paymentWizard.reason
    }

}
