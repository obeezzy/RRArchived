import QtQuick 2.13
import QtQuick.Controls 2.5
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Controls.Material 2.3
import "../rrui" as RRUi
import "../singletons"

TableView {
    id: tableView

    readonly property Row columnHeader: columnHeader
    readonly property Column rowHeader: rowHeader
    property color headerColor: Material.backgroundColor
    property color headerTextColor: Material.foreground

    topMargin: columnHeader.implicitHeight
    leftMargin: rowHeader.implicitWidth
    columnWidthProvider: function (column) { return tableView.model.headerData(column, Qt.Horizontal, Qt.SizeHintRole); }

    TableDelegate {
        x: -width
        y: tableView.contentY
        width: rowHeader.width
        height: 35
        z: 10000

        Rectangle { anchors.fill: parent }
    }

    Row {
        id: columnHeader
        y: tableView.contentY
        z: 2

        Repeater {
            model: tableView.columns > 0 ? tableView.columns : 1

            RRUi.TableDelegate {
                implicitWidth: tableView.columnWidthProvider ? tableView.columnWidthProvider(modelData) : columnLabel.implicitWidth
                implicitHeight: 35

                FluidControls.SubheadingLabel {
                    id: columnLabel
                    anchors.fill: parent
                    text: tableView.model.headerData(modelData, Qt.Horizontal)
                    color: tableView.headerTextColor
                    padding: 0
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: tableView.model.headerData(modelData, Qt.Horizontal, Qt.TextAlignmentRole)
                    elide: Qt.ElideRight
                    font.bold: true

                    background: Rectangle { color: tableView.headerColor }
                }
            }
        }
    }

    Column {
        id: rowHeader
        x: tableView.contentX
        z: 2

        Repeater {
            model: tableView.rows > 0 ? tableView.rows : 1

            RRUi.TableDelegate {
                implicitWidth: 32
                implicitHeight: tableView.rowHeightProvider ? tableView.rowHeightProvider(modelData) : rowLabel.implicitHeight

                FluidControls.SubheadingLabel {
                    id: rowLabel
                    anchors.fill: parent
                    text: tableView.model.headerData(modelData, Qt.Vertical)
                    color: tableView.headerTextColor
                    padding: 10
                    verticalAlignment: Text.AlignVCenter

                    background: Rectangle { color: tableView.headerColor }
                }
            }
        }
    }
}
