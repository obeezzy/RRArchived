import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import "../singletons"

RRUi.Wizard {
    id: newDebtTransactionWizard

    property real amountOwed: 0
    property date dueDate: new Date()
    property var debtModelData: null
    readonly property bool isExistingDebt: debtModelData !== null

    function show(debtModelData) {
        if (debtModelData !== undefined && Object(debtModelData).hasOwnProperty("due_date")) {
            newDebtTransactionWizard.debtModelData = debtModelData;
            newDebtTransactionWizard.dueDate = debtModelData.due_date;
        }

        newDebtTransactionWizard.open();
    }

    width: 640
    height: 580

    onAboutToShow: {
        if (isExistingDebt)
            newDebtTransactionWizard.contentItem.push(Qt.resolvedUrl("newdebttransactionwizard/DueDatePage.qml"),
                                                      { "dueDate": newDebtTransactionWizard.dueDate });
        else
            newDebtTransactionWizard.contentItem.push(Qt.resolvedUrl("newdebttransactionwizard/DebtPage.qml"));
    }
    onClosed: {
        amountOwed = 0;
        dueDate = new Date();
        debtModelData = null;
        newDebtTransactionWizard.contentItem.clear();
    }
}
