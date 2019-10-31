import QtQuick 2.12
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../rrui" as RRUi
import "../singletons"

RRUi.DataTableView {
    id: incomeReportTableView

    property alias busy: incomeReportModel.busy
    property alias autoQuery: incomeReportModel.autoQuery

    signal success(int successCode)
    signal error(int errorCode)

    function refresh() { incomeReportModel.refresh(); }

    flickableDirection: TableView.VerticalFlick

    model: RRModels.IncomeReportModel {
        id: incomeReportModel
        tableViewWidth: incomeReportTableView.widthWithoutMargins
        onSuccess: incomeReportTableView.success(successCode);
        onError: incomeReportTableView.error(errorCode);
    }

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: incomeReportTableView.contentHeight > incomeReportTableView.height
    }

    delegate: DelegateChooser {
        DelegateChoice {
            column: RRModels.IncomeReportModel.PurposeColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: incomeReportTableView.columnHeader.children[column].width
                implicitHeight: incomeReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: purpose
                }
            }
        }

        DelegateChoice {
            column: RRModels.IncomeReportModel.AmountColumn
            delegate: RRUi.TableDelegate {
                implicitWidth: incomeReportTableView.columnHeader.children[column].width
                implicitHeight: incomeReportTableView.rowHeader.children[row].height

                FluidControls.SubheadingLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                    elide: Text.ElideRight
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: Number(amount).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
                }
            }
        }
    }
}
