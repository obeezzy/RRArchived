import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../../../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import "../../../singletons"

RRUi.Wizard {
    id: newDebtTransactionWizard

    property real amountOwed: 0
    property int debtIndex: -1
    property date dueDate: new Date(0)
    readonly property bool isExistingDebt: debtIndex >= 0

    function show(debtIndex, dueDate) {
        if (debtIndex !== undefined && dueDate !== undefined) {
            newDebtTransactionWizard.debtIndex = debtIndex;
            newDebtTransactionWizard.dueDate = dueDate;
        }

        newDebtTransactionWizard.open();
    }

    width: 640
    height: 540

    onAboutToShow: {
        if (isExistingDebt)
            stackView.push(Qt.resolvedUrl("DueDatePage.qml"), { "dueDate": newDebtTransactionWizard.dueDate });
        else
            stackView.push(Qt.resolvedUrl("DebtPage.qml"));
    }
    onClosed: {
        amountOwed = 0;
        dueDate = new Date(0);
        stackView.clear();
    }

    buttonRow: Row {
        spacing: 8

        QQC2.Button {
            text: qsTr("Cancel")
            flat: true
            onClicked: newDebtTransactionWizard.reject();
        }

        QQC2.Button {
            text: {
                switch (stackView.currentObjectName) {
                case "dueDatePage":
                    if (newDebtTransactionWizard.isExistingDebt)
                        qsTr("Update");
                    else
                        qsTr("Finish");
                    break;
                default:
                    qsTr("Next");
                    break;
                }
            }
            flat: true
            enabled: {
                switch (stackView.currentObjectName) {
                case "debtPage":
                    stackView.currentItem.amountOwed > 0;
                    break;
                default:
                    true;
                }
            }
            onClicked: {
                switch (stackView.currentObjectName) {
                case "debtPage":
                    newDebtTransactionWizard.amountOwed = newDebtTransactionWizard.stackView.currentItem.amountOwed;
                    newDebtTransactionWizard.stackView.push(Qt.resolvedUrl("DueDatePage.qml"));
                    break;
                case "dueDatePage":
                    newDebtTransactionWizard.dueDate = newDebtTransactionWizard.stackView.currentItem.dueDate;
                    newDebtTransactionWizard.accept();
                    break;
                }
            }
        }
    }
}
