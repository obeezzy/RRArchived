import QtQuick 2.13
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Controls.Material 2.3
import "../rrui" as RRUi
import "../singletons"

TableView {
    id: dataTableView

    readonly property Row columnHeader: columnHeader
    readonly property Column rowHeader: rowHeader
    readonly property real widthWithoutMargins: width - leftMargin
    property color headerColor: Material.backgroundColor
    property color headerTextColor: Material.foreground

    topMargin: columnHeader.implicitHeight
    leftMargin: rowHeader.implicitWidth
    columnWidthProvider: function (column) { return dataTableView.model.headerData(column, Qt.Horizontal, Qt.SizeHintRole); }

    TableDelegate {
        x: -width
        y: dataTableView.contentY
        width: rowHeader.width
        height: columnHeader.height
        z: 10000
    }

    Row {
        id: columnHeader
        y: dataTableView.contentY
        z: 2
        spacing: dataTableView.columnSpacing
        visible: dataTableView.rows > 0

        Repeater {
            model: dataTableView.columns > 0 ? dataTableView.columns : 1

            RRUi.TableDelegate {
                implicitWidth: dataTableView.columnWidthProvider ? dataTableView.columnWidthProvider(modelData) : columnLabel.implicitWidth
                implicitHeight: dataTableView.rows > 1 ? rowHeader.height / dataTableView.rows : 44

                FluidControls.SubheadingLabel {
                    id: columnLabel
                    anchors.fill: parent
                    text: dataTableView.model.headerData(modelData, Qt.Horizontal)
                    color: dataTableView.headerTextColor
                    topPadding: 10
                    bottomPadding: 10
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: dataTableView.model.headerData(modelData, Qt.Horizontal, Qt.TextAlignmentRole)
                    elide: Qt.ElideRight
                    font.bold: true

                    background: Item { }
                }
            }
        }
    }

    Column {
        id: rowHeader
        x: dataTableView.contentX
        z: 2
        spacing: dataTableView.rowSpacing
        visible: dataTableView.rows > 0

        Repeater {
            model: dataTableView.rows > 0 ? dataTableView.rows : 1

            RRUi.TableDelegate {
                implicitWidth: 32
                implicitHeight: dataTableView.rowHeightProvider ? dataTableView.rowHeightProvider(modelData) : rowLabel.implicitHeight

                FluidControls.SubheadingLabel {
                    id: rowLabel
                    anchors.fill: parent
                    text: dataTableView.model.headerData(modelData, Qt.Vertical)
                    color: dataTableView.headerTextColor
                    padding: 10
                    verticalAlignment: Text.AlignVCenter

                    background: Item { }
                }
            }
        }
    }

    // Row delegates
    ItemDelegate {
        x: -dataTableView.leftMargin
        y: dataTableView.contentY
        width: parent.width + dataTableView.leftMargin
        height: columnHeader.height
        visible: dataTableView.rows > 0

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            color: Stylesheet.tableViewDivider
            height: 1
        }
    }

    ListView {
        x: -dataTableView.leftMargin
        width: dataTableView.width
        height: dataTableView.height
        spacing: dataTableView.rowSpacing
        interactive: false
        clip: true
        visible: dataTableView.rows > 0

        model: dataTableView.rows >= 0 ? dataTableView.rows : 0

        delegate: ItemDelegate {
            width: ListView.view.width
            height: columnHeader.height

            Rectangle {
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                color: Stylesheet.tableViewDivider
                height: 1
            }
        }
    }
}
