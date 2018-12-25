import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import "../singletons"

RRUi.Page {
    id: newItemPage

    property int itemId: -1

    title: itemId == -1 ? qsTr("New stock item") : qsTr("Edit stock item")
    padding: 10

    /*!
        \qmlsignal void goBack(var event)

        This signal is emitted when the back action is triggered or back key is released.

        By default, the page will be popped from the page stack. To change the default
        behavior, for example to show a confirmation dialog, listen for this signal using
        \c onGoBack and set \c event.accepted to \c true. To dismiss the page from your
        dialog without triggering this signal and re-showing the dialog, call
        \c page.forcePop().
    */

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

        transitionComponent: Item {
            id: transitionItem

            Flickable {
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    bottom: buttonCard.top
                    bottomMargin: 12
                }

                contentWidth: detailCard.width
                contentHeight: detailCard.height
                flickableDirection: Flickable.VerticalFlick

                FluidControls.Card {
                    id: detailCard

                    property bool userAddedCategory: false
                    property string categoryText: ""
                    readonly property string defaultCategoryText: qsTr("No category added")

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

                        height: itemInfoColumn.height + 12

                        Column {
                            id: imageColumn
                            anchors {
                                left: parent.left
                                top: parent.top
                            }
                            width: 160
                            spacing: 4

                            RRUi.LetterCircleImage {
                                id: itemImage
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }

                                height: width
                                font.pixelSize: 30
                                name: itemTextField.text

                                FluidControls.Icon {
                                    anchors.centerIn: parent
                                    visible: itemImage.name === ""
                                    color: "white"
                                    source: FluidControls.Utils.iconUrl("image/photo_camera")
                                }
                            }

                            Row {
                                anchors.horizontalCenter: parent.horizontalCenter

                                RRUi.ToolButton {
                                    id: takePhotoButton
                                    icon.source: FluidControls.Utils.iconUrl("image/photo_camera")
                                    text: qsTr("Take a photo")
                                }

                                RRUi.ToolButton {
                                    id: selectPhotoButton
                                    icon.source: FluidControls.Utils.iconUrl("image/photo")
                                    text: qsTr("Select image")
                                }

                                RRUi.ToolButton {
                                    id: deviceSearchButton
                                    icon.source: FluidControls.Utils.iconUrl("hardware/phonelink")
                                    text: qsTr("Start DeviceLink")
                                }
                            }
                        }

                        Column {
                            id: itemInfoColumn
                            anchors {
                                left: imageColumn.right
                                right: parent.right
                                top: parent.top
                                margins: 20
                                leftMargin: 80
                            }
                            spacing: 16

                            RRUi.SectionFragment {
                                id: categoryFragment
                                title: qsTr("Category")
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }

                                Row {
                                    spacing: 0

                                    FluidControls.Icon {
                                        anchors.verticalCenter: parent.verticalCenter
                                        source: FluidControls.Utils.iconUrl("communication/vpn_key")
                                    }

                                    Item { width: 12; height: 1 }

                                    QQC2.ComboBox {
                                        id: categoryComboBox
                                        visible: !categoryLabel.visible
                                        enabled: visible
                                        focus: enabled
                                        width: 220
                                        textRole: "category"
                                        currentIndex: -1
                                        displayText: currentIndex > -1 ? currentText : qsTr("<Select category>")
                                        model: RRModels.StockCategoryModel {
                                            onSuccess: categoryComboBox.currentIndex = categoryComboBox.find(stockItemDetailRecord.category);
                                        }
                                    }

                                    QQC2.Label {
                                        id: categoryLabel
                                        anchors.verticalCenter: parent.verticalCenter
                                        width: 200
                                        font.pixelSize: 17
                                        color: detailCard.categoryText == "" ? Material.color(Material.Grey) : "black"
                                        text: detailCard.defaultCategoryText
                                        visible: detailCard.userAddedCategory || !categoryComboBox.count
                                    }

                                    Item { width: 4; height: 1 }

                                    RRUi.ToolButton {
                                        id: addCategoryButton
                                        anchors.verticalCenter: parent.verticalCenter
                                        icon.source: FluidControls.Utils.iconUrl(detailCard.userAddedCategory ?
                                                                                     "content/remove_circle" : "content/add_circle")
                                        text: detailCard.userAddedCategory ? qsTr("Remove category") : qsTr("Add category")

                                        onClicked: {
                                            if (detailCard.userAddedCategory) {
                                                if (categoryComboBox.count === 0) {
                                                    categoryLabel.text = detailCard.defaultCategoryText;
                                                    detailCard.categoryText = "";
                                                }

                                                detailCard.userAddedCategory = false;
                                            } else {
                                                categoryInputDialogLoader.active = true;
                                            }
                                        }
                                    }

                                    RRUi.ToolButton {
                                        id: searchButton
                                        anchors.verticalCenter: parent.verticalCenter
                                        icon.source: FluidControls.Utils.iconUrl("action/search")
                                        text: qsTr("Search")
                                    }
                                }
                            }

                            RRUi.SectionFragment {
                                id: itemFragment
                                title: qsTr("Product")
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }

                                Row {
                                    spacing: 12

                                    FluidControls.Icon {
                                        anchors.verticalCenter: parent.verticalCenter
                                        source: FluidControls.Utils.iconUrl("communication/vpn_key")
                                    }

                                    RRUi.TextField {
                                        id: itemTextField
                                        width: 200
                                        placeholderText: qsTr("Product name");
                                    }
                                }

                                Row {
                                    spacing: 12

                                    FluidControls.Icon {
                                        anchors.verticalCenter: parent.verticalCenter
                                        source: FluidControls.Utils.iconUrl("communication/vpn_key")
                                    }

                                    RRUi.TextField {
                                        id: descriptionTextField
                                        width: 200
                                        placeholderText: qsTr("Short description");
                                    }
                                }
                            }

                            RRUi.SectionFragment {
                                id: quantityUnitFragment
                                title: qsTr("Quantity and unit")
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }

                                Row {
                                    spacing: 12

                                    FluidControls.Icon {
                                        anchors.verticalCenter: parent.verticalCenter
                                        source: FluidControls.Utils.iconUrl("communication/vpn_key")
                                    }

                                    QQC2.SpinBox {
                                        id: quantitySpinBox
                                        down.indicator: null
                                        up.indicator: null
                                        width: 50
                                        editable: newItemPage.itemId <= 0
                                        to: 10000
                                    }

                                    RRUi.TextField {
                                        id: unitTextField
                                        placeholderText: qsTr("Unit")
                                        width: 200
                                    }
                                }

                                Row {
                                    spacing: 12

                                    QQC2.CheckBox {
                                        id: trackedCheckBox
                                        text: qsTr("Can be tracked")
                                    }

                                    QQC2.CheckBox {
                                        id: divisibleCheckBox
                                        text: qsTr("Can be divided")
                                        checked: true
                                    }
                                }
                            }

                            RRUi.SectionFragment {
                                id: priceFragment
                                title: qsTr("Price")
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }

                                Row {
                                    spacing: 12

                                    FluidControls.Icon {
                                        anchors.verticalCenter: parent.verticalCenter
                                        source: FluidControls.Utils.iconUrl("editor/attach_money")
                                    }

                                    QQC2.SpinBox {
                                        id: retailPriceSpinBox
                                        down.indicator: null
                                        up.indicator: null
                                        width: 50
                                        editable: true
                                        to: 10000

                                        property int decimals: 2
                                        property real realValue: value / 100

                                        validator: DoubleValidator {
                                            bottom: Math.min(retailPriceSpinBox.from, retailPriceSpinBox.to)
                                            top:  Math.max(retailPriceSpinBox.from, retailPriceSpinBox.to)
                                        }

                                        textFromValue: function(value, locale) {
                                            return "\u20a6 " + Number(value / 100).toLocaleString(locale, 'f', retailPriceSpinBox.decimals)
                                        }

                                        valueFromText: function(text, locale) {
                                            return Number.fromLocaleString(locale, text) * 100
                                        }
                                    }
                                }
                            }
                        }
                    }

                    RRModels.StockItemPusher {
                        id: stockItemPusher
                        itemId: newItemPage.itemId
                        imageSource: itemImage.source
                        category: detailCard.categoryText || categoryComboBox.currentText
                        item: itemTextField.text
                        description: descriptionTextField.text
                        quantity: quantitySpinBox.value
                        unit: unitTextField.text
                        categoryNote: ""
                        itemNote: ""
                        costPrice: retailPriceSpinBox.value
                        retailPrice: retailPriceSpinBox.value
                        tracked: trackedCheckBox.checked
                        divisible: divisibleCheckBox.checked

                        onSuccess: {
                            switch (successCode) {
                            case RRModels.StockItemPusher.AddItemSuccess:
                                newItemPage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Your item was successfully added!"));
                                transitionView.trigger();
                                break;
                            case RRModels.StockItemPusher.UpdateItemSuccess:
                                newItemPage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Your item was successfully updated!"));
                                newItemPage.pop();
                                break;
                            }
                        }
                        onError: {
                            switch (errorCode) {
                            case RRModels.StockItemPusher.InsertFailed:
                                failureAlertDialogLoader.message = qsTr("Item could not be inserted into the database.");
                                break;
                            default:
                                failureAlertDialogLoader.message = qsTr("The cause of the error could not be determined.");
                                break;
                            }

                            failureAlertDialogLoader.active = true;
                        }
                    }

                    RRModels.StockItemDetailRecord {
                        id: stockItemDetailRecord
                        autoQuery: newItemPage.itemId > 0
                        itemId: newItemPage.itemId

                        onSuccess: {
                            categoryComboBox.currentIndex = categoryComboBox.find(category);
                            itemTextField.text = item;
                            descriptionTextField.text = description;
                            quantitySpinBox.value = quantity;
                            unitTextField.text = unit;
                            retailPriceSpinBox.value = retailPrice;
                            trackedCheckBox.checked = true;
                            divisibleCheckBox.checked = divisible;
                        }
                    }

                    RRUi.FailureAlertDialogLoader {
                        id: failureAlertDialogLoader
                        parent: QQC2.ApplicationWindow.contentItem
                        title: qsTr("Failed to add item")
                    }

                    RRUi.BusyOverlay { visible: stockItemPusher.busy }

                    Loader {
                        id: categoryInputDialogLoader
                        active: false
                        onLoaded: item.open();

                        sourceComponent: FluidControls.InputDialog {
                            parent: FluidControls.ApplicationWindow.contentItem
                            x: (parent.width - width) / 2
                            y: (parent.height - height) / 2
                            text: qsTr("Enter category name")
                            textField.placeholderText: qsTr("Category")

                            onAccepted: {
                                if (textField.text.trim().length > 0 && categoryComboBox.find(textField.text.trim()) === -1) {
                                    categoryLabel.text = textField.text;
                                    detailCard.categoryText = textField.text;
                                    detailCard.userAddedCategory = true;
                                } else if (categoryComboBox.find(textField.text.trim()) > -1) {
                                    categoryLabel.text = detailCard.defaultCategoryText;
                                    detailCard.categoryText = "";
                                    detailCard.userAddedCategory = false;
                                    categoryComboBox.currentIndex = categoryComboBox.find(textField.text.trim());
                                }
                            }
                            onClosed: categoryInputDialogLoader.active = false;
                        }
                    }

                    function validateUserInput() {
                        if (detailCard.categoryText.trim() === "" && !categoryComboBox.visible) {
                            failureAlertDialogLoader.message = qsTr("Category field is not set.    "); // Force dialog to stretch
                            failureAlertDialogLoader.create();
                            return false;
                        } else if (itemTextField.text.trim() === "") {
                            failureAlertDialogLoader.message = qsTr("Item field is empty.          "); // Force dialog to stretch
                            failureAlertDialogLoader.create();
                            return false;
                        } else if (unitTextField.text.trim() === "") {
                            failureAlertDialogLoader.message = qsTr("Unit field is empty.          "); // Force dialog to stretch
                            failureAlertDialogLoader.create();
                            return false;
                        }

                        return true;
                    }
                }
            }

            FluidControls.Card {
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
                        text: newItemPage.itemId == -1 ? qsTr("Add Item") : qsTr("Update Item")
                        onClicked: if (detailCard.validateUserInput()) stockItemPusher.push();
                    }
                }
            }
        }
    }
}
