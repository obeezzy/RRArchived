import QtQuick 2.12

Item {
    id: receiptTemplate

    property real leftMargin: 0
    property real rightMargin: 0
    property real topMargin: 0
    property real bottomMargin: 0

    property url businessLogoUrl: ""
    property string businessName: ""
    property string businessAddress: ""
    property string businessPhoneNumber: ""
    property int businessEstablishmentYear: 1700

    property url barcodeUrl: ""
    property int receiptNumber: 0
    property date date: new Date()
    property string cashier: ""
    property string paymentType: ""
    property var model: null

    function formatDate(date) { return Qt.formatDateTime(date, "dd/MM/yy h:mm ap"); }

    implicitWidth: 500
    implicitHeight: 500
}
