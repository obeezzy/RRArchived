import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import Qt.labs.qmlmodels 1.0 as QQModels
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import "../singletons"

TableView {
    id: userTableView

    property Component buttonRow: null
    property int keys: RRModels.UserModel.All
    property string filterText: ""
    property int filterColumn: -1
    property var columnWidths: [ 40, width - 280, 100, 120 ]

    enum Columns {
        UserIdColumn,
        UserColumn,
        ActiveColumn,
        PresetColumn,
        ActionColumn
    }

    signal success(int successCode)
    signal error(int errorCode)

    function removeUser(userId) { userTableView.model.removeUser(userId); }
    function refresh() { userTableView.model.refresh(); }
    function undoLastCommit() { userTableView.model.undoLastCommit(); }

    bottomMargin: 20
    clip: true
    visible: !model.busy
    columnWidthProvider: function (column) { return columnWidths[column]; }
    flickableDirection: TableView.VerticalFlick
    columnSpacing: 8

    FluidControls.Placeholder {
        visible: userTableView.rows == 0 && !userTableView.model.busy
        anchors.centerIn: parent
        icon.source: FluidControls.Utils.iconUrl("social/person")
        text: qsTr("No registered users.")
    }

    QQC2.BusyIndicator {
        anchors.centerIn: parent
        visible: userTableView.model.busy
    }

    model: RRModels.UserModel {
        filterText: userTableView.filterText
        filterColumn: userTableView.filterColumn
        keys: userTableView.keys
        onSuccess: userTableView.success(successCode);
        onError: userTableView.error(errorCode);
    }

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: userTableView.contentHeight > userTableView.height
    }

    delegate: QQModels.DelegateChooser {
        QQModels.DelegateChoice {
            column: UserTableView.Columns.UserIdColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: user_id
            }
        }

        QQModels.DelegateChoice {
            column: UserTableView.Columns.UserColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignLeft
                verticalAlignment: Qt.AlignVCenter
                text: user
            }
        }

        QQModels.DelegateChoice {
            column: UserTableView.Columns.ActiveColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: active
            }
        }

        QQModels.DelegateChoice {
            column: UserTableView.Columns.PresetColumn
            delegate: FluidControls.SubheadingLabel {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: rank
            }
        }

        QQModels.DelegateChoice {
            column: UserTableView.Columns.ActionColumn
            delegate: Loader {
                readonly property var modelData: {
                    "client_id": model.client_id,
                    "transaction_id": model.transaction_id
                }

                sourceComponent: userTableView.buttonRow
            }
        }
    }
}
