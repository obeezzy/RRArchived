import "../../rrui" as RRUi
import QtQuick 2.12

RRUi.SubView {
    id: debtTransactionSubView

    property int debtorId: 0
    property alias listView: debtTransactionListView

    contentItem: FocusScope {
        DebtTransactionListView {
            id: debtTransactionListView

            anchors.fill: parent
            debtorId: debtTransactionSubView.debtorId
        }

    }

}
