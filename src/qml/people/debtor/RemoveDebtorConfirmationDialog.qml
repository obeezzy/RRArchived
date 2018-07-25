import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../../rrui" as RRUi

RRUi.AlertDialog {
    id: removeDebtorConfirmationDialog

    property var modelData: null

    width: 300
    text: qsTr("Are you sure you want to remove this debtor?");
    standardButtons: RRUi.AlertDialog.Yes | RRUi.AlertDialog.No
    onClosed: deleteConfirmationDialog.modelData = null;

    function show(modelData) {
        if (Object(modelData).hasOwnProperty("debtor_id")) {
            text = qsTr("Are you sure you want to remove '%1' from the list?").arg(modelData.preferred_name);
            removeDebtorConfirmationDialog.modelData = modelData;
            open();
        }
    }
}
