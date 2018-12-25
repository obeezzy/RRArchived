import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.3

TextField {
    id: itemTextField

    ToolTip.visible: hovered && placeholderText !== ""
    ToolTip.delay: text === "" ? 1500 : 300
    ToolTip.text: placeholderText
}
