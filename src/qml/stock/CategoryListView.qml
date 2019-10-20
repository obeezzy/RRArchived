import QtQuick 2.12
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 2.12 as QQC2
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

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: categoryListView.contentHeight > categoryListView.height
    }

    delegate: RRUi.Card {
        width: ListView.view.width
        height: column.height
        Material.elevation: 0

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

            RRUi.DataTableView {
                id: stockItemTableView
                anchors {
                    left: parent.left
                    right: parent.right
                    topMargin: 20
                    bottomMargin: 20
                }

                height: contentHeight + topMargin + bottomMargin
                model: item_model
                columnSpacing: 8
                flickableDirection: TableView.VerticalFlick
                interactive: false

                delegate: DelegateChooser {
                    DelegateChoice {
                        column: RRModels.StockItemModel.ImageColumn
                        delegate: RRUi.TableDelegate {
                            implicitWidth: stockItemTableView.columnHeader.children[RRModels.StockItemModel.ImageColumn].width
                            implicitHeight: stockItemTableView.rowHeader.children[0].height

                            RRUi.LetterCircleImage {
                                anchors {
                                    centerIn: parent
                                    verticalCenterOffset: -4
                                }
                                name: model.item
                                source: model.image_source
                                sourceSize: Qt.size(width, height)
                            }
                        }
                    }

                    DelegateChoice {
                        column: RRModels.StockItemModel.ItemColumn
                        delegate: RRUi.TableDelegate {
                            implicitWidth: stockItemTableView.columnHeader.children[RRModels.StockItemModel.ItemColumn].width
                            implicitHeight: stockItemTableView.rowHeader.children[0].height

                            FluidControls.SubheadingLabel {
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                    verticalCenter: parent.verticalCenter
                                }

                                horizontalAlignment: Qt.AlignLeft
                                verticalAlignment: Qt.AlignVCenter
                                text: item
                            }
                        }
                    }

                    DelegateChoice {
                        column: RRModels.StockItemModel.QuantityColumn
                        delegate: RRUi.TableDelegate {
                            implicitWidth: stockItemTableView.columnHeader.children[RRModels.StockItemModel.QuantityColumn].width
                            implicitHeight: stockItemTableView.rowHeader.children[0].height

                            FluidControls.SubheadingLabel {
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                    verticalCenter: parent.verticalCenter
                                }

                                horizontalAlignment: Qt.AlignRight
                                verticalAlignment: Qt.AlignVCenter
                                text: quantity + " " + unit
                            }
                        }
                    }

                    DelegateChoice {
                        column: RRModels.StockItemModel.CostPriceColumn
                        delegate: RRUi.TableDelegate {
                            implicitWidth: stockItemTableView.columnHeader.children[RRModels.StockItemModel.CostPriceColumn].width
                            implicitHeight: stockItemTableView.rowHeader.children[0].height

                            FluidControls.SubheadingLabel {
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                    verticalCenter: parent.verticalCenter
                                }

                                horizontalAlignment: Qt.AlignRight
                                verticalAlignment: Qt.AlignVCenter
                                text: Number(cost_price).toLocaleCurrencyString(Qt.locale("en_NG"))
                            }
                        }
                    }

                    DelegateChoice {
                        column: RRModels.StockItemModel.ActionColumn
                        delegate: RRUi.TableDelegate {
                            implicitWidth: stockItemTableView.columnHeader.children[RRModels.StockItemModel.ActionColumn].width
                            implicitHeight: stockItemTableView.rowHeader.children[0].height

                            Loader {
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

                                anchors.centerIn: parent
                                sourceComponent: categoryListView.buttonRow
                            }
                        }
                    }
                }

//                add: Transition {
//                    PropertyAction { property: "height"; value: 0 }
//                    PropertyAction { property: "opacity"; value: 0 }

//                    SequentialAnimation {
//                        NumberAnimation { property: "height"; to: 40; duration: 300; easing.type: Easing.InOutQuad }
//                        NumberAnimation { property: "opacity"; to: 1; duration: 300; easing.type: Easing.OutCubic }
//                    }
//                }

//                displaced: Transition {
//                    SequentialAnimation {
//                        PauseAnimation { duration: 125 }
//                        NumberAnimation { property: "y"; easing.type: Easing.InOutQuad }
//                    }
//                }
//                remove: Transition {
//                    SequentialAnimation {
//                        PauseAnimation { duration: 125 }
//                        NumberAnimation { property: "height"; to: 0; easing.type: Easing.InOutQuad }
//                    }
//                }

//                Behavior on height {
//                    SequentialAnimation {
//                        PauseAnimation { duration: 125 }
//                        NumberAnimation { }
//                    }
//                }
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
