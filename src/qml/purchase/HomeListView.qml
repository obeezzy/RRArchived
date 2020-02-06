import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../common"
import "homecards"
import "../rrui" as RRUi

RRUi.HomeListViewBase {
    id: homeListView

    topMargin: 40
    bottomMargin: 40
    spacing: 16

    model: RRModels.PurchaseHomeModel { }
    delegate: Loader {
        width: ListView.view.width
        active: false
        onLoaded: item.model = data_model;

        source: {
            switch (data_type) {
            case "last_purchased_products":
                Qt.resolvedUrl("homecards/LastPurchasedItemCard.qml")
                break;
            default:
                undefined
            }
        }
    }
}
