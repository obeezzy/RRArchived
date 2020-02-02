import QtQuick 2.12
import "../../rrui" as RRUi

RRUi.SubView {
    id: debtTransactionSubView

    property int debtorId: -1
    property alias listView: debtTransactionListView

    contentItem: FocusScope {
        DebtTransactionListView {
            id: debtTransactionListView
            anchors.fill: parent
            debtorId: debtTransactionSubView.debtorId
        }
    }
}
