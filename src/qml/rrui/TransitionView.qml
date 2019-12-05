import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2

QQC2.StackView {
    id: transitionView

    property Component component: null
    readonly property Item item: currentItem

    initialItem: component

    function trigger() { transitionView.replace(null, transitionView.component); }
}
