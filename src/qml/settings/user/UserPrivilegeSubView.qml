import "../../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import com.gecko.rr.models 1.0 as RRModels

RRUi.SubView {
    id: userPrivilegeSubView

    property int userId: 0
    readonly property alias listView: listView

    contentItem: FocusScope {
        UserPrivilegeListView {
            id: listView

            anchors.fill: parent
            userId: userPrivilegeSubView.userId
        }

    }

}
