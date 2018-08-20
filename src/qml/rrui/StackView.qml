import QtQuick 2.10
import QtQuick.Controls 2.3

StackView {
    id: stackView

    readonly property string currentObjectName: currentItem != null ? currentItem.objectName : ""
}
