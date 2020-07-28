import "../../rrui" as RRUi
import "../../singletons"
import Fluid.Controls 1.0 as FluidControls
import QtCharts 2.3 as QCharts
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts

RRUi.HomeCard {
    id: lastIncomeEntryCard

    cardTitle: qsTr("Last expense entries")
    subtitle: qsTr("Top 5")

    QQLayouts.RowLayout {
        height: 300

        anchors {
            left: parent.left
            right: parent.right
        }

    }

}
