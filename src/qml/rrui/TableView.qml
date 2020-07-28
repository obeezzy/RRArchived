import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12

Item {
    id: tableView

    property list<TableViewColumn> columns
    property Component headerDelegate: null
    property Component cellDelegate: null
    property Component rowDelegate
    property alias interactive: listView.interactive
    property alias headerPositioning: listView.headerPositioning
    property real cellSpacing: 0
    property alias model: listView.model

    implicitWidth: 500
    implicitHeight: 500

    ListView {
        id: rowListView

        anchors.fill: parent
        model: tableView.model
        contentY: listView.contentY
        interactive: false

        header: Item {
            width: ListView.view.width
            height: listView.headerItem.height

            Loader {
                anchors.fill: parent
                sourceComponent: tableView.rowDelegate
            }

        }

        delegate: Loader {
            width: ListView.view.width
            height: listView.contentItem.children.length > 0 ? listView.contentItem.children[0].height : 0
            sourceComponent: tableView.rowDelegate
        }

    }

    ListView {
        id: listView

        anchors.fill: parent

        header: Row {
            spacing: tableView.cellSpacing

            Repeater {
                model: tableView.columns

                delegate: Loader {
                    readonly property var headerData: {
                        "title": tableView.columns[index].title
                    }

                    width: tableView.columns[index].width
                    sourceComponent: tableView.headerDelegate
                }

            }

        }

        delegate: Row {
            id: listViewDelegate

            readonly property int row: index

            spacing: tableView.cellSpacing

            Repeater {
                model: columns.length

                delegate: Loader {
                    readonly property int column: index
                    readonly property var cellData: {
                        "modelData": listView.model.get(listViewDelegate.row)[tableView.columns[column].role],
                        "row": listViewDelegate.row,
                        "column": column,
                        "textRole": tableView.columns[column].role
                    }

                    width: listViewDelegate.ListView.view.headerItem.children[column].width
                    sourceComponent: tableView.cellDelegate
                }

            }

        }

    }

    rowDelegate: FluidControls.ListItem {
        showDivider: true
    }

}
