import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import QtCharts 2.2 as QCharts
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls

ListView {
    id: tableView

    property var headerData: null
    property var colorModel: null
    signal rowEntered(int row)
    signal rowExited(int row)

    implicitWidth: headerItem.width
    implicitHeight: 400

    contentWidth: headerItem.width

    header: Column {
        function itemAt(index) { return repeater.itemAt(index) }

        spacing: 0
        width: tableView.width

        Row {
            spacing: 1
            Repeater {
                id: repeater
                model: tableView.headerData

                FluidControls.SubheadingLabel {
                    text: modelData.title !== undefined ? modelData.title : ""
                    width: modelData.width === undefined ? contentWidth : modelData.width
                    horizontalAlignment: modelData.horizontalAlignment
                }
            }
        }

        FluidControls.ThinDivider {
            anchors {
                left: parent.left
                right: parent.right
            }
        }
    }

    model: null
    delegate: FluidControls.ListItem {
        id: delegate
        readonly property int row: model.index

        width: ListView.view.width
        height: 30
        showDivider: true

        Row {
            anchors {
                top: parent.top
                bottom: parent.bottom
            }
            spacing: 1

            Repeater {
                model: tableView.headerData

                Loader {
                    readonly property int column: index

                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }

                    sourceComponent: {
                        if (modelData.type === "color")
                            rectDelegate
                        else if (modelData.type === "date")
                            dateDelegate
                        else if (modelData.type === "money")
                            moneyDelegate
                        else
                            textDelegate
                    }

                    Component {
                        id: textDelegate

                        FluidControls.CaptionLabel {
                            verticalAlignment: modelData.verticalAlignment !== undefined ? modelData.verticalAlignment : Qt.AlignVCenter
                            horizontalAlignment: modelData.horizontalAlignment !== undefined ? modelData.horizontalAlignment : Qt.AlignLeft
                            text: tableView.model.get(delegate.row)[modelData.role] !== undefined ? tableView.model.get(delegate.row)[modelData.role] : ""
                            width: modelData.width !== undefined ? modelData.width : tableView.headerItem.itemAt(column).width
                            clip: true
                        }
                    }

                    Component {
                        id: rectDelegate
                        Item {
                            readonly property int column: index

                            visible: tableView.colorModel !== null && column === 0
                            width: modelData.width !== undefined ? modelData.width : 20
                            height: modelData.width !== undefined ? modelData.width : 20

                            Rectangle {
                                color: tableView.colorModel[delegate.row] !== undefined ? tableView.colorModel[delegate.row] : "white"
                                anchors.centerIn: parent
                                width: 10
                                height: width
                                radius: width / 2
                            }
                        }
                    }

                    Component {
                        id: dateDelegate

                        FluidControls.CaptionLabel {
                            verticalAlignment: modelData.verticalAlignment !== undefined ? modelData.verticalAlignment : Qt.AlignVCenter
                            horizontalAlignment: modelData.horizontalAlignment !== undefined ? modelData.horizontalAlignment : Qt.AlignLeft
                            text: tableView.model.get(delegate.row)[modelData.role] !== undefined ? Qt.formatDate(tableView.model.get(delegate.row)[modelData.role], modelData.format) : ""
                            width: modelData.width !== undefined ? modelData.width : tableView.headerItem.itemAt(column).width
                            clip: true
                        }
                    }

                    Component {
                        id: moneyDelegate

                        FluidControls.CaptionLabel {
                            verticalAlignment: modelData.verticalAlignment !== undefined ? modelData.verticalAlignment : Qt.AlignVCenter
                            horizontalAlignment: modelData.horizontalAlignment !== undefined ? modelData.horizontalAlignment : Qt.AlignLeft
                            text: tableView.model.get(delegate.row)[modelData.role] !== undefined ? Number(tableView.model.get(delegate.row)[modelData.role]).toLocaleCurrencyString(Qt.locale("en_NG")) : ""
                            width: modelData.width !== undefined ? modelData.width : tableView.headerItem.itemAt(column).width
                            clip: true
                        }
                    }
                }
            }
        }

        onHoveredChanged: {
            if (hovered)
                tableView.rowEntered(index);
            else
                tableView.rowExited(index);
        }
    }

    QQC2.ScrollIndicator.horizontal: QQC2.ScrollIndicator { }
    QQC2.ScrollIndicator.vertical: QQC2.ScrollIndicator { }
}
