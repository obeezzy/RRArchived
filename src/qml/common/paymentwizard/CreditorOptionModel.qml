import QtQuick 2.12

ListModel {
    id: creditorOptionModel

    property bool canAcceptAlternatePaymentMethod: false

    ListElement { option: "give_change"; text: qsTr("Give the customer as change."); }
    ListElement { option: "create_creditor"; text: qsTr("Add the customer to the list of people I owe."); }
    ListElement { option: "overlook_balance"; text: qsTr("Overlook this balance. (Not recommended)"); }
}
