import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels

RRUi.HomeListViewBase {
    id: dashboardListView

    topMargin: 20
    bottomMargin: 20
    spacing: 24

    model: RRModels.DashboardHomeModel {
    }

    delegate: RRUi.HomeCard {
        width: ListView.view.width
        cardTitle: title
        shortDescription: short_description
        imageUrl: image_url
        iconUrl: icon_url
        breadcrumb: breadcrumbs
        onViewRequested: dashboardListView.viewRequested(breadcrumbs, {
        })
    }

}
