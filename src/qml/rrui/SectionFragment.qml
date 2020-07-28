import "../common"
import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import com.gecko.rr.models 1.0 as RRModels

Item {
    id: sectionFragment

    property string title: ""
    property bool dividerVisible: true
    default property alias content: dataColumn.data

    implicitWidth: 200
    implicitHeight: column.height + divider.height

    Column {
        id: column

        spacing: 12

        anchors {
            left: parent.left
            right: parent.right
        }

        FluidControls.HeadlineLabel {
            text: sectionFragment.title

            anchors {
                left: parent.left
                right: parent.right
            }

        }

        Column {
            id: dataColumn

            anchors {
                left: parent.left
                right: parent.right
            }

        }

        Rectangle {
            id: divider

            height: 1
            color: "lightgray"
            visible: sectionFragment.dividerVisible

            anchors {
                left: parent.left
                right: parent.right
            }

        }

    }

}
