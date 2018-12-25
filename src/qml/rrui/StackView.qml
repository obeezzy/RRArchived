import QtQuick 2.12
import QtQuick.Controls 2.12

StackView {
    id: stackView

    readonly property string currentObjectName: currentItem != null ? currentItem.objectName : ""
}
