import QtQuick 2.10

ListModel {
    ListElement {
        title: qsTr("Dashboard")
        iconName: "action/home"
        fileName: "common/DashboardPage.qml"
    }

    ListElement {
        title: qsTr("Stock")
        iconName: "editor/pie_chart"
        fileName: "stock/HomePage.qml"
    }

    ListElement {
        title: qsTr("Sales")
        iconName: ""
        fileName: "sales/HomePage.qml"
    }

    ListElement {
        title: qsTr("Purchase")
        iconName: ""
        fileName: "common/DashboardPage.qml"
    }

    ListElement {
        title: qsTr("Income")
        iconName: ""
        fileName: "common/DashboardPage.qml"
    }

    ListElement {
        title: qsTr("Expenses")
        iconName: ""
        fileName: "common/DashboardPage.qml"
    }

    ListElement {
        title: qsTr("Debtors")
        iconName: ""
        fileName: "people/debtor/HomePage.qml"
    }

    ListElement {
        title: qsTr("Creditors")
        iconName: ""
        fileName: "common/DashboardPage.qml"
    }

    ListElement {
        title: qsTr("Upload")
        iconName: ""
        fileName: "common/DashboardPage.qml"
    }

    ListElement {
        title: qsTr("Settings")
        iconName: ""
        fileName: "common/DashboardPage.qml"
    }
}
