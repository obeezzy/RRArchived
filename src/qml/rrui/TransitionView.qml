import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2

QQC2.StackView {
    id: transitionView

    property Component transitionComponent: null

    initialItem: transitionComponent

    function trigger() { transitionView.replace(null, transitionView.transitionComponent); }
}
