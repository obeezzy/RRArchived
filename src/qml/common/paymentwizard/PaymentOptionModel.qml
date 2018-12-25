import QtQuick 2.12

ListModel {
    id: paymentOptionModel

    property bool canAcceptCash: true
    property bool canAcceptCard: true

    ListElement { iconName: "editor/attach_money"; option: "cash"; text: qsTr("Cash"); visible: true; checked: false }
    ListElement { iconName: "action/credit_card"; option: "card"; text: qsTr("Debit/credit card"); visible: true; checked: false }
    ListElement { iconName: "device/access_time"; option: "none"; text: qsTr("Pay later"); visible: true; checked: false }

    Component.onCompleted: {
        if (!paymentOptionModel.canAcceptCash)
            paymentOptionModel.remove(0);
        else if (!paymentOptionModel.canAcceptCard)
            paymentOptionModel.remove(1);
    }
}
