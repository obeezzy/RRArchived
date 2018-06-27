import QtQuick 2.9
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels

ListView {
    id: debtorListView

    property Component buttonRow: null
    property string filterText: ""
    property int filterColumn: -1

    topMargin: 20
    bottomMargin: 20
    clip: true
    visible: !model.busy

    model: RRModels.DebtorModel {
        filterText: debtorListView.filterText
        filterColumn: debtorListView.filterColumn
    }

    QQC2.ScrollBar.vertical: QQC2.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: debtorListView.contentHeight > debtorListView.height
        size: .3
        width: 5
        contentItem: Rectangle {
            color: Material.color(Material.Grey)
            radius: width / 2
        }
    }

    delegate: FluidControls.ListItem {
        width: ListView.view.width
        height: 40
        text: name
        showDivider: true

        leftItem: FluidControls.CircleImage {
            anchors.verticalCenter: parent.verticalCenter
            width: 30
            height: 30
        }

        rightItem: Loader {
            id: rightButtonLoader

            readonly property var modelData: {
                "client_id": client_id
            }

            anchors.verticalCenter: parent.verticalCenter
            sourceComponent: debtorListView.buttonRow
        }
    }

    function refresh() { debtorListView.model.refresh(); }
}
