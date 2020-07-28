import Qt.labs.qmlmodels 1.0 as QQModels
import QtQuick 2.12
import com.gecko.rr.models 1.0 as RRModels

ReceiptTemplate {
    id: receiptTypeA

    height: column.height

    Rectangle {
        anchors.fill: parent
    }

    Column {
        id: column

        topPadding: receiptTypeA.topMargin
        bottomPadding: receiptTypeA.bottomMargin

        anchors {
            left: parent.left
            right: parent.right
        }

        // Barcode
        ReceiptText {
            text: receiptTypeA.barcodeUrl
            visible: receiptTypeA.barcodeUrl !== ""

            anchors {
                left: parent.left
                right: parent.right
            }

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
            text: receiptTypeA.businessName
            visible: receiptTypeA.businessName !== ""

            anchors {
                left: parent.left
                right: parent.right
            }

        }

        // Receipt number
        ReceiptText {
            text: qsTr("Receipt no: %1").arg(receiptTypeA.receiptNumber)
            visible: receiptTypeA.receiptNumber != 0

            anchors {
                left: parent.left
                right: parent.right
            }

        }

        // Date/time
        ReceiptText {
            text: qsTr("Date: %1").arg(receiptTypeA.formatDate(receiptTypeA.date))
            visible: receiptTypeA.date

            anchors {
                left: parent.left
                right: parent.right
            }

        }

        // Cashier
        ReceiptText {
            text: qsTr("Cashier: %1").arg(receiptTypeA.cashier)
            visible: receiptTypeA.cashier !== ""

            anchors {
                left: parent.left
                right: parent.right
            }

        }

        // Payment type
        ReceiptText {
            text: qsTr("Payment type: %1").arg(receiptTypeA.paymentType)
            visible: receiptTypeA.paymentType !== ""

            anchors {
                left: parent.left
                right: parent.right
            }

        }

        Item {
            width: 1
            height: 8
        }

        // Cart items
        ReceiptCartTableView {
            visible: rows !== 0
            model: receiptTypeA.cartModel

            anchors {
                left: parent.left
                right: parent.right
            }

        }

    }

}
