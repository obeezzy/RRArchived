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
        tableViewWidth: categoryListView.width
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

            ItemTableView {
                anchors {
                    left: parent.left
                    right: parent.right
                    topMargin: 20
                    bottomMargin: 20
                }
                model: item_model
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
