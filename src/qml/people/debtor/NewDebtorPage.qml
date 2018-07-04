import QtQuick 2.10
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import com.gecko.rr.models 1.0 as RRModels
import "../../rrui" as RRUi
import "../../common"

RRUi.Page {
    id: newDebtorPage

    property RRModels.DebtorModel model: null

    title: qsTr("Add debtor")
    topPadding: 10
    bottomPadding: 10
    leftPadding: 20
    rightPadding: 20

    contentItem: FocusScope {
        FluidControls.Card {
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                bottom: parent.bottom
            }

            width: 800
        }
    }
}
