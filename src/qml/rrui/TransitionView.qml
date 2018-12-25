import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2

QQC2.StackView {
    id: transitionView

    property Component transitionComponent: null

    initialItem: transitionComponent

    function trigger() { transitionView.replace(null, transitionView.transitionComponent); }
}
