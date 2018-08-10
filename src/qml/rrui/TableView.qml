import QtQuick 2.10
import Fluid.Controls 1.0 as FluidControls

Item {
    id: tableView

    implicitWidth: 500
    implicitHeight: 500

    property list<TableViewColumn> columns
    property Component headerDelegate: null
    property Component itemDelegate: null
    property Component rowDelegate: FluidControls.ListItem { showDivider: true }
    property alias interactive: itemListView.interactive
    property alias headerPositioning: itemListView.headerPositioning

    property alias model: itemListView.model

    ListView {
        id: rowListView
        anchors.fill: parent

        model: tableView.model
        contentY: itemListView.contentY
        interactive: false

        header: Item {
            width: ListView.view.width
            height: itemListView.headeritem.height

            Loader {
                anchors.fill: parent
                sourceComponent: tableView.rowDelegate
            }
        }

        delegate: Loader {
            width: ListView.view.width
            height: itemListView.contentItem.children.length > 0 ? itemListView.contentItem.children[0].height : 0
            sourceComponent: tableView.rowDelegate
        }
    }

    ListView {
        id: itemListView
        anchors.fill: parent

        header: Row {
            Repeater {
                model: tableView.columns
                delegate: Loader {
                    readonly property var styleData: {
                        "title": tableView.columns[index].title
                    }

                    width: tableView.columns[index].width
                    sourceComponent: tableView.headerDelegate
                }
            }
        }

        delegate: Row {
            id: itemListViewDelegate

            readonly property int row: index

            Repeater {
                model: columns.length
                delegate: Loader {
                    readonly property int column: index
                    readonly property var styleData: {
                        "modelData": itemListView.model.get(itemListView.row)[tableView.columns[index].role],
                                "row": itemListViewDelegate.row,
                                "column": column
                    }
                    width: itemListViewDelegate.ListView.view.headerItem.children[index].width
                    sourceComponent: tableView.itemDelegate
                }
            }
        }
    }
}
