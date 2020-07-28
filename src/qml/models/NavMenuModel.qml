import QtQuick 2.12

ListModel {
    ListElement {
        title: qsTr("Dashboard")
        iconUrl: "qrc:/icons/home.svg"
        fileName: "common/DashboardPage.qml"
    }

    ListElement {
        title: qsTr("Reports")
        iconUrl: "qrc:/icons/chart-areaspline.svg"
        fileName: "reports/HomePage.qml"
    }

    ListElement {
        title: qsTr("Stock")
        iconUrl: "qrc:/icons/truck.svg"
        fileName: "stock/HomePage.qml"
    }

    ListElement {
        title: qsTr("Sales")
        iconUrl: "qrc:/icons/coin.svg"
        fileName: "sales/HomePage.qml"
    }

    ListElement {
        title: qsTr("Purchases")
        iconUrl: "qrc:/icons/cart.svg"
        fileName: "purchase/HomePage.qml"
    }

    ListElement {
        title: qsTr("Income")
        iconUrl: "qrc:/icons/cash-multiple.svg"
        fileName: "income/HomePage.qml"
    }

    ListElement {
        title: qsTr("Expenses")
        iconUrl: "qrc:/icons/wallet.svg"
        fileName: "expense/HomePage.qml"
    }

    ListElement {
        title: qsTr("Debtors")
        iconUrl: "qrc:/icons/account-star.svg"
        fileName: "people/debtor/HomePage.qml"
    }

    ListElement {
        title: qsTr("Creditors")
        iconUrl: "qrc:/icons/account-star-outline.svg"
        fileName: "people/creditor/HomePage.qml"
    }

    ListElement {
        title: qsTr("Settings")
        iconUrl: "qrc:/icons/tune.svg"
        fileName: "settings/HomePage.qml"
    }

}
