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
    bottomMargin: 100
    spacing: 16

    model: RRModels.SaleHomeModel { }
    delegate: Loader {
        width: ListView.view.width
        active: true
        onLoaded: item.model = data_model;

        source: {
            switch (data_type) {
            case "total_revenue":
                Qt.resolvedUrl("homecards/TotalRevenueCard.qml")
                break;
            case "most_sold_items":
                Qt.resolvedUrl("homecards/MostSoldItemCard.qml")
                break;
            default:
                undefined
            }
        }
    }
}
