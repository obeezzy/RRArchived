import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Layouts 1.3 as QQLayouts
import com.gecko.rr.components 1.0 as RRComponents
import "../rrui" as RRUi

RRUi.Dialog {
    id: cartItemEditorDialog

    property int itemIndex: -1
    property int itemId: -1
    property string item: ""
    property real quantity: 0
    property real maximumQuantity: 10
    property string unit: ""
    property real unitPrice: 0
    property real cost: 0

    implicitWidth: 480
    implicitHeight: 640
    title: qsTr("Edit \"%1\"").arg(item)

    function show(itemInfo) {
        if (Object(itemInfo).hasOwnProperty("item_index"))
            cartItemEditorDialog.itemIndex = itemInfo.item_index;
        if (Object(itemInfo).hasOwnProperty("item_id"))
            cartItemEditorDialog.itemId = itemInfo.item_id;
        if (Object(itemInfo).hasOwnProperty("item"))
            cartItemEditorDialog.item = itemInfo.item;
        if (Object(itemInfo).hasOwnProperty("quantity"))
            cartItemEditorDialog.quantity = itemInfo.quantity;
        if (Object(itemInfo).hasOwnProperty("available_quantity"))
            cartItemEditorDialog.maximumQuantity = itemInfo.available_quantity;
        if (Object(itemInfo).hasOwnProperty("unit"))
            cartItemEditorDialog.unit = itemInfo.unit;
        if (Object(itemInfo).hasOwnProperty("unit_price"))
            cartItemEditorDialog.unitPrice = itemInfo.unit_price;
        if (Object(itemInfo).hasOwnProperty("cost"))
            cartItemEditorDialog.cost = itemInfo.cost;

        open();
    }

    contentItem: FocusScope {
        focus: cartItemEditorDialog.visible
        onActiveFocusChanged: if (activeFocus) quantityTextField.focus = true;

        FluidControls.SubheadingLabel {
            id: quantityLeftLabel
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: grid.top
                bottomMargin: 32
            }
            text: qsTr("This product has %1 %2 left.").arg(cartItemEditorDialog.maximumQuantity).arg(cartItemEditorDialog.unit)
            font.italic: true
            color: "darkgray"
        }

        Grid {
            id: grid
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
                    text: cartItemEditorDialog.quantity
                    width: Math.min(150, implicitWidth)
                    horizontalAlignment: Qt.AlignRight

                    validator: RRComponents.DoubleValidator {
                        bottom: 0
                        top: cartItemEditorDialog.maximumQuantity
                        decimals: 4
                    }
                    onActiveFocusChanged: if (activeFocus) selectAll();
                    onTextChanged: cartItemEditorDialog.cost = cartItemEditorDialog.unitPrice * cartItemEditorDialog.quantity;
                    onTextEdited: cartItemEditorDialog.quantity = Number(text);
                }

                FluidControls.SubheadingLabel {
                    id: unitLabel
                    height: quantityTextField.height

                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: cartItemEditorDialog.unit
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
                text: cartItemEditorDialog.unitPrice
                horizontalAlignment: Qt.AlignRight
                onTextEdited: {
                    cartItemEditorDialog.unitPrice = Number(text).toFixed(2);
                    cartItemEditorDialog.cost = cartItemEditorDialog.unitPrice * cartItemEditorDialog.quantity;
                }

                validator: RRComponents.DoubleValidator {
                    bottom: 0
                    top: Number.MAX_VALUE
                    decimals: 2
                }

                onActiveFocusChanged: if (activeFocus) selectAll();
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
                text: cartItemEditorDialog.cost
                horizontalAlignment: Qt.AlignRight
                onTextEdited: {
                    cartItemEditorDialog.cost = Number(text);
                    cartItemEditorDialog.unitPrice = Number(cartItemEditorDialog.cost / cartItemEditorDialog.quantity).toFixed(2);
                }

                validator: RRComponents.DoubleValidator {
                    bottom: 0
                    top: Number.MAX_VALUE
                    decimals: 2
                }

                onActiveFocusChanged: if (activeFocus) selectAll();
            }
        }
    }
}
