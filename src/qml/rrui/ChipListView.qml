import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3

ListView {
    id: chipListView

    signal clicked(int index)

    spacing: 8
    orientation: ListView.Horizontal
    interactive: false

    delegate: RRUi.Chip {
        text: modelData
        onClicked: chipListView.clicked(index)
    }

}
