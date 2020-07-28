import "../rrui" as RRUi
import "../singletons"
import Fluid.Controls 1.0 as FluidControls
import Qt.labs.qmlmodels 1.0
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels

RRUi.DataTableView {
    id: expenseReportTableView

    property alias busy: expenseReportModel.busy
    property alias autoQuery: expenseReportModel.autoQuery

    signal success(var result)
    signal error(var result)

    function refresh() {
        expenseReportModel.refresh();
    }

    flickableDirection: TableView.VerticalFlick

    model: RRModels.ExpenseReportModel {
        id: expenseReportModel

        tableViewWidth: expenseReportTableView.widthWithoutMargins
        onSuccess: expenseReportTableView.success(result)
        onError: expenseReportTableView.error(result)
    }

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: expenseReportTableView.contentHeight > expenseReportTableView.height
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.ExpenseReportModel.PurposeColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: expenseReportTableView.columnHeader.children[column].width
                implicitHeight: expenseReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: purpose

                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                }

            }

        }

        DelegateChoice {
            column: RRModels.ExpenseReportModel.AmountColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: expenseReportTableView.columnHeader.children[column].width
                implicitHeight: expenseReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: Number(amount).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))

                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                }

            }

        }

    }

}
