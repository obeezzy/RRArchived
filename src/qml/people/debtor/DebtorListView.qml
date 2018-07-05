import QtQuick 2.9
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import "../../singletons"

ListView {
    id: debtorListView

    property Component buttonRow: null
    property string filterText: ""
    property int filterColumn: -1

    signal success(int successCode)
    signal error(int errorCode)

    topMargin: 20
    bottomMargin: 20
    clip: true
    visible: !model.busy

    model: RRModels.DebtorModel {
        filterText: debtorListView.filterText
        filterColumn: debtorListView.filterColumn
        onSuccess: debtorListView.success(successCode);
        onError: debtorListView.error(errorCode);
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
        text: preferred_name
        showDivider: true

        leftItem: RRUi.LetterCircleImage {
            anchors.verticalCenter: parent.verticalCenter
            name: preferred_name
        }

        rightItem: Row {
            spacing: 8

            FluidControls.SubheadingLabel {
                text: Number(model.total_debt).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocale))
                verticalAlignment: Qt.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            Loader {
                id: rightButtonLoader

                readonly property var modelData: {
                    "client_id": model.client_id,
                            "debtor_id": model.debtor_id,
                            "preferred_name": model.preferred_name
                }

                anchors.verticalCenter: parent.verticalCenter
                sourceComponent: debtorListView.buttonRow
            }
        }
    }

    add: Transition {
        NumberAnimation { property: "y"; from: 100; duration: 300; easing.type: Easing.OutCubic }
        NumberAnimation { property: "opacity"; to: 1; duration: 300; easing.type: Easing.OutCubic }
    }

    remove: Transition {
        NumberAnimation { property: "opacity"; to: 0; duration: 300; easing.type: Easing.OutCubic }
    }

    removeDisplaced: Transition {
        NumberAnimation { properties: "x,y"; duration: 300 }
    }

    function refresh() { debtorListView.model.refresh(); }
    function undoLastCommit() { debtorListView.model.undoLastCommit(); }
}
