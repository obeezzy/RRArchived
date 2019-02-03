import QtQuick 2.12
import Qt.labs.qmlmodels 1.0 as QQModels
import com.gecko.rr.models 1.0 as RRModels

ReceiptTemplate {
    id: receiptTypeA

    height: column.height

    Rectangle { anchors.fill: parent }

    Column {
        id: column
        anchors {
            left: parent.left
            right: parent.right
        }
        topPadding: receiptTypeA.topMargin
        bottomPadding: receiptTypeA.bottomMargin

        // Barcode
        ReceiptText {
            anchors {
                left: parent.left
                right: parent.right
            }
            text: receiptTypeA.barcodeUrl
            visible: receiptTypeA.barcodeUrl !== ""
        }

        // Business logo
        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            width: Math.min(parent.width, implicitWidth)
            height: Math.min(300, implicitHeight)
            source: receiptTypeA.businessLogoUrl
            visible: source !== ""
        }

        // Business name
        ReceiptText {
            anchors {
                left: parent.left
                right: parent.right
            }
            text: receiptTypeA.businessName
            visible: receiptTypeA.businessName !== ""
        }

        // Receipt number
        ReceiptText {
            anchors {
                left: parent.left
                right: parent.right
            }
            text: qsTr("Receipt no: %1").arg(receiptTypeA.receiptNumber)
            visible: receiptTypeA.receiptNumber != 0
        }

        // Date/time
        ReceiptText {
            anchors {
                left: parent.left
                right: parent.right
            }
            text: qsTr("Date: %1").arg(receiptTypeA.formatDate(receiptTypeA.date))
            visible: receiptTypeA.date
        }

        // Cashier
        ReceiptText {
            anchors {
                left: parent.left
                right: parent.right
            }
            text: qsTr("Cashier: %1").arg(receiptTypeA.cashier)
            visible: receiptTypeA.cashier !== ""
        }

        // Payment type
        ReceiptText {
            anchors {
                left: parent.left
                right: parent.right
            }
            text: qsTr("Payment type: %1").arg(receiptTypeA.paymentType)
            visible: receiptTypeA.paymentType !== ""
        }

        Item { width: 1; height: 8 }

        // Cart items
        ReceiptCartTableView {
            anchors {
                left: parent.left
                right: parent.right
            }
            visible: rows !== 0
            model: receiptTypeA.cartModel
        }
    }
}
