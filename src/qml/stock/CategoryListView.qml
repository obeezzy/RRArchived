import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels

ListView {
    id: categoryListView

    property Component buttonRow: null
    property string filterText: ""
    property int filterColumn: -1

    signal success(int successCode)
    signal error(int errorCode)

    function refresh() { categoryListView.model.refresh(); }
    function undoLastCommit() { categoryListView.model.undoLastCommit(); }

    topMargin: 20
    bottomMargin: 20
    clip: true
    visible: !model.busy

    model: RRModels.StockCategoryItemModel {
        filterText: categoryListView.filterText
        filterColumn: categoryListView.filterColumn
        onSuccess: categoryListView.success(successCode);
        onError: categoryListView.error(errorCode);
    }

    QQC2.ScrollBar.vertical: QQC2.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: categoryListView.contentHeight > categoryListView.height
        size: .3
        width: 5
        contentItem: Rectangle {
            color: Material.color(Material.Grey)
            radius: width / 2
        }
    }

    delegate: Item {
        width: ListView.view.width
        height: column.height

        Rectangle { anchors.fill: parent }

        Column {
            id: column
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            Item {
                visible: index !== 0
                width: 1
                height: 24
            }

            FluidControls.HeadlineLabel {
                id: titleLabel
                text: category
            }

            ListView {
                anchors {
                    left: parent.left
                    right: parent.right
                    topMargin: 20
                    bottomMargin: 20
                }

                height: contentHeight
                model: item_model

                delegate: FluidControls.ListItem {
                    id: itemListItem
                    width: ListView.view.width
                    height: 40
                    showDivider: true
                    clip: true
                    padding: 0

                    QQLayouts.RowLayout {
                        anchors.fill: parent
                        spacing: 16

                        RRUi.LetterCircleImage {
                            QQLayouts.Layout.alignment: Qt.AlignVCenter
                            name: model.item
                        }

                        FluidControls.SubheadingLabel {
                            QQLayouts.Layout.alignment: Qt.AlignVCenter
                            QQLayouts.Layout.fillWidth: true
                            text: item
                        }

                        Loader {
                            id: rightButtonLoader

                            readonly property var modelData: {
                                "category_id": category_id,
                                "category": category,
                                "item_id": model.item_id,
                                "item": model.item,
                                "unit_id": model.unit_id,
                                "unit": model.unit,
                                "quantity": model.quantity,
                                "retail_price": model.retail_price,
                                "cost_price": model.cost_price
                            }

                            QQLayouts.Layout.fillHeight: true
                            QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                            sourceComponent: categoryListView.buttonRow
                        }
                    }
                }

                add: Transition {
                    PropertyAction { property: "height"; value: 0 }
                    PropertyAction { property: "opacity"; value: 0 }

                    SequentialAnimation {
                        NumberAnimation { property: "height"; to: 40; duration: 300; easing.type: Easing.InOutQuad }
                        NumberAnimation { property: "opacity"; to: 1; duration: 300; easing.type: Easing.OutCubic }
                    }
                }

                displaced: Transition {
                    SequentialAnimation {
                        PauseAnimation { duration: 125 }
                        NumberAnimation { property: "y"; easing.type: Easing.InOutQuad }
                    }
                }
                remove: Transition {
                    SequentialAnimation {
                        PauseAnimation { duration: 125 }
                        NumberAnimation { property: "height"; to: 0; easing.type: Easing.InOutQuad }
                    }
                }

                Behavior on height {
                    SequentialAnimation {
                        PauseAnimation { duration: 125 }
                        NumberAnimation { }
                    }
                }
            }
        }
    }

    add: Transition {
        NumberAnimation { property: "y"; from: 100; duration: 300; easing.type: Easing.OutCubic }
        NumberAnimation { property: "opacity"; to: 1; duration: 300; easing.type: Easing.OutCubic }
    }

    displaced: Transition {
        SequentialAnimation {
            PauseAnimation { duration: 125 }
            NumberAnimation { property: "y"; easing.type: Easing.InOutQuad }
        }
    }
    remove: Transition {
        SequentialAnimation {
            PauseAnimation { duration: 125 }
            NumberAnimation { property: "height"; to: 0; easing.type: Easing.InOutQuad }
        }
    }
}
