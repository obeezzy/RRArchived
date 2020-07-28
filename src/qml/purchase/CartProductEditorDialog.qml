import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import com.gecko.rr.components 1.0 as RRComponents

RRUi.Dialog {
    id: cartProductEditorDialog

    property int productIndex: -1
    property int productId: 0
    property string product: ""
    property real quantity: 0
    readonly property real maximumQuantity: 1e+07
    property string unit: ""
    property real unitPrice: 0
    property real cost: 0

    function show(product) {
        if (Object(product).hasOwnProperty("product_index"))
            cartProductEditorDialog.productIndex = product.product_index;

        if (Object(product).hasOwnProperty("product_id"))
            cartProductEditorDialog.productId = product.product_id;

        if (Object(product).hasOwnProperty("product"))
            cartProductEditorDialog.product = product.product;

        if (Object(product).hasOwnProperty("quantity"))
            cartProductEditorDialog.quantity = product.quantity;

        if (Object(product).hasOwnProperty("unit"))
            cartProductEditorDialog.unit = product.unit;

        if (Object(product).hasOwnProperty("unit_price"))
            cartProductEditorDialog.unitPrice = product.unit_price;

        if (Object(product).hasOwnProperty("cost"))
            cartProductEditorDialog.cost = product.cost;

        open();
    }

    implicitWidth: 480
    implicitHeight: 640
    title: qsTr("Edit \"%1\"").arg(item)

    contentItem: FocusScope {
        focus: cartProductEditorDialog.visible
        onActiveFocusChanged: if (activeFocus) {
            quantityTextField.focus = true;
        }

        Grid {
            anchors.centerIn: parent
            rowSpacing: 8
            columnSpacing: 8
            columns: 2

            FluidControls.SubheadingLabel {
                id: quantityLabel

                height: quantityTextField.height
                text: qsTr("Quantity:")
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
            }

            Row {
                spacing: 8

                RRUi.TextField {
                    id: quantityTextField

                    topPadding: 12
                    focus: true
                    text: cartProductEditorDialog.quantity
                    width: Math.min(150, implicitWidth)
                    horizontalAlignment: Qt.AlignRight
                    onActiveFocusChanged: if (activeFocus) {
                        selectAll();
                    }
                    onTextChanged: cartProductEditorDialog.cost = cartProductEditorDialog.unitPrice * cartProductEditorDialog.quantity
                    onTextEdited: cartProductEditorDialog.quantity = Number(text)

                    validator: RRComponents.DoubleValidator {
                        bottom: 0
                        top: cartProductEditorDialog.maximumQuantity
                        decimals: 4
                    }

                }

                FluidControls.SubheadingLabel {
                    id: unitLabel

                    height: quantityTextField.height
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: cartProductEditorDialog.unit
                }

            }

            FluidControls.SubheadingLabel {
                id: unitPriceLabel

                text: qsTr("Unit price:")
                height: unitPriceTextField.height
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
            }

            RRUi.TextField {
                id: unitPriceTextField

                topPadding: 12
                width: Math.min(150, implicitWidth)
                text: cartProductEditorDialog.unitPrice
                horizontalAlignment: Qt.AlignRight
                onTextEdited: {
                    cartProductEditorDialog.unitPrice = Number(text).toFixed(2);
                    cartProductEditorDialog.cost = cartProductEditorDialog.unitPrice * cartProductEditorDialog.quantity;
                }
                onActiveFocusChanged: if (activeFocus) {
                    selectAll();
                }

                validator: RRComponents.DoubleValidator {
                    bottom: 0
                    top: Number.MAX_VALUE
                    decimals: 2
                }

            }

            FluidControls.SubheadingLabel {
                id: costLabel

                text: qsTr("Cost:")
                height: costTextField.height
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
            }

            RRUi.TextField {
                id: costTextField

                topPadding: 12
                width: Math.min(150, implicitWidth)
                text: Number(cartProductEditorDialog.cost).toFixed()
                horizontalAlignment: Qt.AlignRight
                onTextEdited: {
                    cartProductEditorDialog.cost = Number(text);
                    cartProductEditorDialog.unitPrice = Number(cartProductEditorDialog.cost / cartProductEditorDialog.quantity).toFixed(2);
                }
                onActiveFocusChanged: if (activeFocus) {
                    selectAll();
                }

                validator: RRComponents.DoubleValidator {
                    bottom: 0
                    top: Number.MAX_VALUE
                    decimals: 2
                }

            }

        }

    }

}
