import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.12 as QQLayouts
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
    property bool autoQuery: true

    signal success(var result)
    signal error(var result)

    topMargin: 20
    bottomMargin: 20
    clip: true
    visible: !model.busy

    model: RRModels.DebtorModel {
        filterText: debtorListView.filterText
        filterColumn: debtorListView.filterColumn
        autoQuery: debtorListView.autoQuery
        onSuccess: debtorListView.success(result);
        onError: debtorListView.error(result);
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
        showDivider: true

        QQLayouts.RowLayout {
            anchors.fill: parent
            spacing: 16

            RRUi.LetterCircleImage {
                QQLayouts.Layout.alignment: Qt.AlignVCenter
                name: preferred_name
            }

            FluidControls.SubheadingLabel {
                QQLayouts.Layout.alignment: Qt.AlignVCenter
                QQLayouts.Layout.fillWidth: true
                text: preferred_name
            }

            Row {
                spacing: 8

                FluidControls.SubheadingLabel {
                    text: Number(model.total_debt).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
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

                    QQLayouts.Layout.fillHeight: true
                    QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                    sourceComponent: debtorListView.buttonRow
                }
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
