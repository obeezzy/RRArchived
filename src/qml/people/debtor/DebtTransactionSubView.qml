import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import com.gecko.rr.models 1.0 as RRModels
import com.gecko.rr.components 1.0 as RRComponents
import "../../rrui" as RRUi
import "../../common"

QQC2.Control {
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
