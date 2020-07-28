import "../common"
import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels
import "homecards"

RRUi.HomeListViewBase {
    id: homeListView

    topMargin: 40
    bottomMargin: 40
    spacing: 16

    model: RRModels.IncomeHomeModel {
    }

    delegate: Loader {
        width: ListView.view.width
        active: false
        onLoaded: item.model = data_model
        source: {
            switch (data_type) {
            case "last_income_entry":
                Qt.resolvedUrl("homecards/LastIncomeEntryCard.qml");
                break;
            default:
                undefined;
            }
        }
    }

}
