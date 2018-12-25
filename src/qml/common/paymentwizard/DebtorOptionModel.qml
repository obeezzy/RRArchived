import QtQuick 2.12

ListModel {
    id: debtorOptionModel

    property bool canAcceptAlternatePaymentMethod: false

    ListElement { option: "create_debtor"; text: qsTr("Add the customer to the list of people who owe me."); }
    ListElement { option: "pay_another_way"; text: qsTr("Pay the balance using a different method."); }
    ListElement { option: "overlook_balance"; text: qsTr("Overlook this balance. (Not recommended)"); }

    Component.onCompleted: if (!debtorOptionModel.canAcceptAlternatePaymentMethod) debtorOptionModel.remove(1);
}
