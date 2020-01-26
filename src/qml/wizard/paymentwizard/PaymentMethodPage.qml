import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../../rrui" as RRUi
import ".."

RRUi.WizardPage {
    id: paymentMethodPage
    objectName: "paymentMethodPage"

    property string selectedOption: paymentMethodPage.canAcceptCash ? "cash" : "card"
    property bool canAcceptCash: true
    property bool canAcceptCard: true
    property RRModels.SaleCartModel cartModel: null
    property int reason: -1

    title: qsTr("Choose payment method")
    leftPadding: 4
    hasNext: true

    onNext: {
        switch (paymentMethodPage.selectedOption) {
        case "cash":
            paymentMethodPage.nextPage.component = Qt.resolvedUrl("PaymentByCashPage.qml");
            paymentMethodPage.nextPage.properties = {
                "totalCost": paymentMethodPage.cartModel.balance,
                "reason": paymentMethodPage.reason,
                "cartModel": paymentMethodPage.cartModel
            };
            break;
        case "card":
            console.warn("TODO: Handle card payment!");
            break;
        default:
            paymentMethodPage.nextPage.component = Qt.resolvedUrl("PaymentCustomerDetailPage.qml");
            paymentMethodPage.nextPage.properties = {
                "customerName": paymentMethodPage.cartModel.customerName,
                "customerPhoneNumber": paymentMethodPage.cartModel.customerPhoneNumber,
                "paymentModel": paymentMethodPage.cartModel.paymentModel,
                "cartModel": paymentMethodPage.cartModel
            };
            break;
        }
    }

    contentItem: FocusScope {
        QQC2.ButtonGroup { id: buttonGroup }

        Column {
            Repeater {
                anchors.fill: parent
                model: PaymentOptionModel {
                    canAcceptCash: paymentMethodPage.canAcceptCash
                    canAcceptCard: paymentMethodPage.canAcceptCard
                }

                delegate: QQC2.RadioButton {
                    id: radioButton
                    checked: index === 0
                    text: model.text
                    QQC2.ButtonGroup.group: buttonGroup

                    onClicked: paymentMethodPage.selectedOption = model.option;

                    contentItem: Row {
                        leftPadding: radioButton.indicator.width + radioButton.spacing + 8
                        spacing: 8

                        FluidControls.Icon {
                            anchors.verticalCenter: parent.verticalCenter
                            source: FluidControls.Utils.iconUrl(model.iconName)
                            size: FluidControls.Units.iconSizes.medium
                        }

                        FluidControls.BodyLabel {
                            anchors.verticalCenter: parent.verticalCenter
                            text: radioButton.text
                            font: radioButton.font
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }
    }
}
