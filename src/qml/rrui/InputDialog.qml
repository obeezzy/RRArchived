import QtQuick 2.12
import Fluid.Controls 1.0 as FluidControls

FluidControls.InputDialog {
    id: inputDialog
    parent: MainWindow !== undefined ? MainWindow.contentItem : null
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
}
