import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import "../singletons"

QQC2.Page {
    id: wizardPage
    property bool hasPrevious: true
    property bool hasNext: false
    property bool nextEnabled: true
    property bool previousEnabled: true
    property string nextButtonText: hasNext ? qsTr("Next") : qsTr("Finish")
    readonly property var nextPage: {
        "component": null,
        "properties": null
    }

    signal next
    signal previous

    function finish() { wizardPage.QQC2.StackView.view.finished(); }
}
