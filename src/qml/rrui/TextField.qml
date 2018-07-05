import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3

TextField {
    id: itemTextField

    ToolTip.visible: hovered
    ToolTip.delay: text === "" ? 1500 : 300
    ToolTip.text: placeholderText
}
