import QtQuick 2.9
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi

ListView {
    id: chipListView

    signal clicked(int index)

    spacing: 8
    orientation: ListView.Horizontal
    interactive: false

    delegate: RRUi.Chip { text: modelData; onClicked: chipListView.clicked(index); }
}
