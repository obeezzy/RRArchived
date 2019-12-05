import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import "fragments" as Fragments
import "../singletons"

RRUi.Page {
    id: newItemPage

    readonly property bool isExistingItem: itemId !== -1
    property int itemId: -1

    title: isExistingItem ? qsTr("Edit product") : qsTr("New product")
    padding: 10

    onGoBack: {
        if (transitionView.item.dirty && !isExistingItem) {
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

            readonly property bool dirty: itemImage.dirty || itemDetails.dirty
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

                        height: itemDetails.height + 12

                        Fragments.ItemImage {
                            id: itemImage
                            anchors {
                                left: parent.left
                                top: parent.top
                            }
                            name: itemDetails.item
                            source: itemDetails.imageUrl
                        }

                        Fragments.ItemDetails {
                            id: itemDetails
                            anchors {
                                left: itemImage.right
                                right: parent.right
                                top: parent.top
                                margins: 20
                                leftMargin: 80
                            }
                            itemId: newItemPage.itemId
                        }
                    }

                    RRModels.StockItemPusher {
                        id: stockItemPusher
                        itemId: newItemPage.itemId
                        imageSource: itemImage.source
                        category: itemDetails.category
                        item: itemDetails.item
                        description: itemDetails.description
                        quantity: itemDetails.quantity
                        unit:itemDetails.unit
                        categoryNote: itemDetails.categoryNote
                        itemNote: itemDetails.itemNote
                        costPrice: itemDetails.costPrice
                        retailPrice: itemDetails.retailPrice
                        tracked: itemDetails.tracked
                        divisible: itemDetails.divisible

                        onSuccess: {
                            switch (successCode) {
                            case RRModels.StockItemPusher.AddItemSuccess:
                                MainWindow.snackBar.show(qsTr("Your item was successfully added!"));
                                transitionView.trigger();
                                break;
                            case RRModels.StockItemPusher.UpdateItemSuccess:
                                MainWindow.snackBar.show(qsTr("Your item was successfully updated!"));
                                newItemPage.pop();
                                break;
                            }
                        }
                        onError: {
                            switch (errorCode) {
                            case RRModels.StockItemPusher.InsertFailed:
                                errorDialog.show(qsTr("Product could not be inserted into the database."),
                                                 qsTr("Failed to add item"));
                                break;
                            case RRModels.StockItemPusher.DuplicateEntryError:
                                errorDialog.show(qsTr("A product of the same name already exists."),
                                                 qsTr("Failed to add item"));
                                break;
                            case RRModels.StockItemPusher.ImageTooLargeError:
                                errorDialog.show(qsTr("The image selected for the product is too large. Please choose an image less than 2 MB."),
                                                 qsTr("Failed to add item"));
                                break;
                            default:
                                errorDialog.show(qsTr("The cause of the error could not be determined."),
                                                 qsTr("Failed to add item"));
                                break;
                            }
                        }
                    }

                    RRUi.ErrorDialog { id: errorDialog }

                    RRUi.BusyOverlay { visible: stockItemPusher.busy }
                }
            }

            function validateUserInput() {
                if (itemDetails.category.trim().length === 0) {
                    errorDialog.show(qsTr("Category field is not set."),
                                     qsTr("Failed to add item")); // Force dialog to stretch
                    return false;
                } else if (itemDetails.item.trim().length === 0) {
                    errorDialog.show(qsTr("Item field is empty."),
                                     qsTr("Failed to add item")); // Force dialog to stretch
                    return false;
                } else if (itemDetails.unit.trim().length === 0) {
                    errorDialog.show(qsTr("Unit field is empty."),
                                     qsTr("Failed to add item")); // Force dialog to stretch
                    return false;
                }

                return true;
            }

            function submit() {
                if (validateUserInput())
                    stockItemPusher.push();
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
                    onClicked: newItemPage.pop();
                }

                QQC2.Button {
                    id: addItemButton
                    Material.elevation: 1
                    text: newItemPage.isExistingItem ? qsTr("Update Product") : qsTr("Add Product")
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
        onAccepted: newItemPage.forcePop();
    }
}
