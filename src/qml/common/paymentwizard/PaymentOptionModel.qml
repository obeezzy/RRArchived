import QtQuick 2.10

ListModel {
    id: paymentOptionModel

    ListElement { iconName: "editor/attach_money"; option: "cash"; text: qsTr("Cash"); visible: true; checked: false }
    ListElement { iconName: "action/credit_card"; option: "card"; text: qsTr("Debit/credit card"); visible: true; checked: false }
    ListElement { iconName: "device/access_time"; option: "none"; text: qsTr("Pay later"); visible: true; checked: false }

    Component.onCompleted: {
    }
}
