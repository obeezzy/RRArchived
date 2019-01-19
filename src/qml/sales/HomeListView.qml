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

    model: RRModels.SaleHomeModel { }
    delegate: Loader {
        readonly property var modelData: data_model
        width: ListView.view.width
        active: false

        sourceComponent: {
            switch (data_type) {
            case "total_revenue":
                totalRevenueCard
                break;
            case "most_sold_items":
                mostSoldItemCard
                break;
            default:
                null
            }
        }
    }

    Component {
        id: totalRevenueCard

        TotalRevenueCard { model: parent.modelData }
    }

    Component {
        id: mostSoldItemCard

        MostSoldItemCard { model: parent.modelData }
    }
}
