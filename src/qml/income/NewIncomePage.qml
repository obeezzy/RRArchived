import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import "../singletons"

RRUi.Page {
    id: newIncomePage

    title: qsTr("New income entry")
    padding: 10

    /*!
        \qmlsignal void goBack(var event)

        This signal is emitted when the back action is triggered or back key is released.

        By default, the page will be popped from the page stack. To change the default
        behavior, for example to show a confirmation dialog, listen for this signal using
        \c onGoBack and set \c event.accepted to \c true. To dismiss the page from your
        dialog without triggering this signal and re-showing the dialog, call
        \c page.forcePop().
    */

    actions: FluidControls.Action {
        icon.source: FluidControls.Utils.iconUrl("action/note_add")
        toolTip: qsTr("Add note")
        text: qsTr("Add note")
    }

    RRUi.TransitionView {
        id: transitionView
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        width: 800

        component: Item {
            id: transitionItem

            Flickable {
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    bottom: buttonCard.top
                    bottomMargin: 12
                }

                contentWidth: detailCard.width
                contentHeight: detailCard.height
                flickableDirection: Flickable.VerticalFlick

                RRUi.Card {
                    id: detailCard

                    padding: 32
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: transitionItem.QQC2.StackView.view.width
                    height: focusScope.height + topPadding + bottomPadding + 50
                    Material.elevation: 2

                    FocusScope {
                        id: focusScope
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        height: itemInfoColumn.height + 12

                        Column {
                            id: itemInfoColumn
                            anchors {
                                left: parent.left
                                right: parent.right
                                top: parent.top
                                margins: 20
                                leftMargin: 80
                            }
                            spacing: 16

                            RRUi.SectionFragment {
                                id: clientFragment
                                title: qsTr("Client details")
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }

                                RRUi.IconTextField {
                                    id: clientTextField
                                    icon.source: FluidControls.Utils.iconUrl("social/person")
                                    textField.placeholderText: qsTr("Name");
                                }
                            }

                            RRUi.SectionFragment {
                                id: paymentMethodFragment
                                title: qsTr("Method of payment")
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }

                                RRUi.IconRadioButton {
                                    id: cashRadioButton
                                    icon.source: Qt.resolvedUrl("qrc:/icons/cash-usd.svg")
                                    text: qsTr("Cash")
                                    checked: true
                                }

                                RRUi.IconRadioButton {
                                    id: debitCardRadioButton
                                    icon.source: FluidControls.Utils.iconUrl("action/credit_card")
                                    text: qsTr("Debit card")
                                }

                                RRUi.IconRadioButton {
                                    id: creditCardRadioButton
                                    icon.source: FluidControls.Utils.iconUrl("action/credit_card")
                                    text: qsTr("Credit card")
                                }
                            }

                            RRUi.SectionFragment {
                                id: transactionFragment
                                title: qsTr("Transaction details")
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }

                                RRUi.IconTextField {
                                    id: purposeTextField
                                    icon.source: Qt.resolvedUrl("qrc:/icons/file-document-box-outline.svg")
                                    textField.placeholderText: qsTr("Purpose");
                                }


                                Row {
                                    spacing: 12

                                    FluidControls.Icon {
                                        anchors.verticalCenter: parent.verticalCenter
                                        source: Qt.resolvedUrl("qrc:/icons/cash-usd.svg")
                                    }

                                    QQC2.SpinBox {
                                        id: amountSpinBox
                                        down.indicator: null
                                        up.indicator: null
                                        width: 50
                                        editable: true
                                        to: 10000

                                        property int decimals: 2
                                        property real realValue: value / 100

                                        validator: DoubleValidator {
                                            bottom: Math.min(amountSpinBox.from, amountSpinBox.to)
                                            top:  Math.max(amountSpinBox.from, amountSpinBox.to)
                                        }

                                        textFromValue: function(value, locale) {
                                            return "\u20a6 " + Number(value).toLocaleString(locale, 'f', amountSpinBox.decimals)
                                        }

                                        valueFromText: function(text, locale) {
                                            return Number.fromLocaleString(locale, text.replace("\u20a6 ", ""))
                                        }
                                    }
                                }
                            }
                        }
                    }

                    RRModels.IncomePusher {
                        id: incomePusher
                        clientName: clientTextField.text
                        purpose: purposeTextField.text
                        amount: amountSpinBox.value
                        paymentMethod: creditCardRadioButton.checked ? RRModels.IncomePusher.CreditCard
                                                                     : debitCardRadioButton.checked ? RRModels.IncomePusher.DebitCard
                                                                                                    : RRModels.IncomePusher.Cash

                        onSuccess: {
                            switch (successCode) {
                            case RRModels.IncomePusher.AddIncomeSuccess:
                                newIncomePage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Your item was successfully added!"));
                                transitionView.trigger();
                                break;
                            case RRModels.IncomePusher.UpdateIncomeSuccess:
                                newIncomePage.RRUi.ApplicationWindow.window.snackBar.show(qsTr("Your item was successfully updated!"));
                                newIncomePage.pop();
                                break;
                            }
                        }
                        onError: {
                            switch (errorCode) {
                            case RRModels.IncomePusher.AddIncomeError:
                                failureAlertDialogLoader.message = qsTr("Failed to add income.");
                                break;
                            default:
                                failureAlertDialogLoader.message = qsTr("The cause of the error could not be determined.");
                                break;
                            }

                            failureAlertDialogLoader.active = true;
                        }
                    }

                    RRUi.FailureAlertDialogLoader {
                        id: failureAlertDialogLoader
                        parent: QQC2.ApplicationWindow.contentItem
                        title: qsTr("Failed to add item")
                    }

                    RRUi.BusyOverlay { visible: incomePusher.busy }

                    function validateUserInput() {
                        if (clientTextField.text.trim() === "") {
                            failureAlertDialogLoader.message = qsTr("Name field is empty.           "); // Force dialog to stretch
                            failureAlertDialogLoader.create();
                            return false;
                        } else if (purposeTextField.text.trim() === "") {
                            failureAlertDialogLoader.message = qsTr("Purpose field is empty.        "); // Force dialog to stretch
                            failureAlertDialogLoader.create();
                            return false;
                        } else if (amountSpinBox.value === 0) {
                            failureAlertDialogLoader.message = qsTr("Amount field cannot be 0. "); // Force dialog to stretch
                            failureAlertDialogLoader.create();
                            return false;
                        }

                        return true;
                    }
                }
            }

            RRUi.Card {
                id: buttonCard
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                height: buttonRow.height
                leftPadding: 20
                rightPadding: 20

                QQC2.Button {
                    id: advancedButton
                    flat: true
                    anchors {
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                    }
                    text: qsTr("Advanced")
                }

                Row {
                    id: buttonRow
                    spacing: 12
                    anchors.right: parent.right

                    QQC2.Button {
                        id: cancelButton
                        flat: true
                        text: qsTr("Cancel")
                        onClicked: newIncomePage.pop();
                    }

                    QQC2.Button {
                        id: addItemButton
                        Material.elevation: 1
                        text: qsTr("Submit")
                        onClicked: if (detailCard.validateUserInput()) incomePusher.push();
                    }
                }
            }
        }
    }
}
