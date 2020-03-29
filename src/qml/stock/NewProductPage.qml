import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "fragments" as Fragments
import "../singletons"

RRUi.Page {
    id: newProductPage

    readonly property bool isExistingProduct: productId > 0
    property int productId: 0

    title: isExistingProduct ? qsTr("Edit product") : qsTr("New product")
    padding: 10

    onGoBack: {
        if (transitionView.item.dirty && !isExistingProduct) {
            leaveConfirmationDialog.open();
            event.accepted = true;
        } else {
            event.accepted = false;
        }
    }

    actions: FluidControls.Action {
        icon.source: FluidControls.Utils.iconUrl("action/note_add")
        toolTip: qsTr("Add note")
        text: qsTr("Add note")
    }

    RRUi.TransitionView {
        id: transitionView
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        width: 800

        component: Item {
            id: transitionItem

            readonly property bool dirty: productImage.dirty || productDetails.dirty
            property url imageUrl: ""

            Flickable {
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    bottom: bottom.bottom
                    bottomMargin: 12
                }

                contentWidth: detailCard.width
                contentHeight: detailCard.height
                flickableDirection: Flickable.VerticalFlick

                RRUi.Card {
                    id: detailCard

                    padding: 32
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: transitionItem.QQC2.StackView.view.width
                    height: focusScope.height + topPadding + bottomPadding + 50
                    Material.elevation: 2

                    FocusScope {
                        id: focusScope
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        height: productDetails.height + 12

                        Fragments.ProductImage {
                            id: productImage
                            anchors {
                                left: parent.left
                                top: parent.top
                            }
                            name: productDetails.product
                            source: productDetails.imageUrl
                        }

                        Fragments.ProductDetails {
                            id: productDetails
                            anchors {
                                left: productImage.right
                                right: parent.right
                                top: parent.top
                                margins: 20
                                leftMargin: 80
                            }
                            productId: newProductPage.productId
                        }
                    }

                    RRModels.StockProductPusher {
                        id: stockProductPusher
                        productId: newProductPage.productId
                        imageUrl: productImage.source
                        category: productDetails.category
                        product: productDetails.product
                        description: productDetails.description
                        quantity: productDetails.quantity
                        unit: productDetails.unit
                        categoryNote: productDetails.categoryNote
                        productNote: productDetails.productNote
                        costPrice: productDetails.costPrice
                        retailPrice: productDetails.retailPrice
                        tracked: productDetails.tracked
                        divisible: productDetails.divisible

                        onSuccess: {
                            switch (result.code) {
                            case RRModels.StockProductPusher.AddProductSuccess:
                                MainWindow.snackBar.show(qsTr("Product was successfully added!"));
                                transitionView.trigger();
                                break;
                            case RRModels.StockProductPusher.UpdateProductSuccess:
                                MainWindow.snackBar.show(qsTr("Product was successfully updated!"));
                                newProductPage.pop();
                                break;
                            }
                        }
                        onError: {
                            switch (result.code) {
                            case RRModels.StockProductPusher.DuplicateEntryError:
                                errorDialog.show(qsTr("A product of the same name already exists."),
                                                 qsTr("Failed to add product"));
                                break;
                            case RRModels.StockProductPusher.ImageTooLargeError:
                                errorDialog.show(qsTr("The image selected for the product is too large. "
                                                      + "Please choose an image less than 2 MB."),
                                                 qsTr("Failed to add product"));
                                break;
                            default:
                                errorDialog.show(qsTr("Product could not be inserted into the database."),
                                                 qsTr("Failed to add product"));
                                break;
                            }
                        }
                    }

                    RRUi.ErrorDialog { id: errorDialog }

                    RRUi.BusyOverlay { visible: stockProductPusher.busy }
                }
            }

            function validateUserInput() {
                if (productDetails.category.trim().length === 0) {
                    errorDialog.show(qsTr("Category field is not set."),
                                     qsTr("Failed to add product")); // Force dialog to stretch
                    return false;
                } else if (productDetails.product.trim().length === 0) {
                    errorDialog.show(qsTr("Product field is empty."),
                                     qsTr("Failed to add product")); // Force dialog to stretch
                    return false;
                } else if (productDetails.unit.trim().length === 0) {
                    errorDialog.show(qsTr("Unit field is empty."),
                                     qsTr("Failed to add product")); // Force dialog to stretch
                    return false;
                }

                return true;
            }

            function submit() {
                if (validateUserInput())
                    stockProductPusher.push();
            }
        }

        RRUi.Card {
            id: buttonCard
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            height: buttonRow.height
            leftPadding: 20
            rightPadding: 20

            QQC2.Button {
                id: advancedButton
                flat: true
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }
                text: qsTr("Advanced")
            }

            Row {
                id: buttonRow
                spacing: 12
                anchors.right: parent.right

                QQC2.Button {
                    id: cancelButton
                    flat: true
                    text: qsTr("Cancel")
                    onClicked: newProductPage.pop();
                }

                QQC2.Button {
                    id: addProductButton
                    Material.elevation: 1
                    text: newProductPage.isExistingProduct ? qsTr("Update Product") : qsTr("Add Product")
                    onClicked: transitionView.item.submit();
                }
            }
        }
    }

    FluidControls.AlertDialog {
        id: leaveConfirmationDialog
        title: Stylesheet.padText(qsTr("Leave?"))
        text: qsTr("Are you sure you want to leave?")
        standardButtons: QQC2.Dialog.Yes | QQC2.Dialog.No
        onAccepted: newProductPage.forcePop();
    }
}
