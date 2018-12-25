import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import "../rrui" as RRUi

RRUi.TextField {
    id: dropdownField

    property var model: null
    property string textRole: ""
    property string subTextRole: ""
    readonly property alias count: listView.count

    signal itemSelected(var modelData)

    onActiveFocusChanged: togglePopupDisplay();
//    onTextEdited: togglePopupDisplay();

    function togglePopupDisplay() {
        if (activeFocus && dropdownField.model != null && !dropdownField.model.busy && popup.contentItem.count > 0) {
            popup.open();
        } else {
            popup.close();
        }
    }

    QQC2.Popup {
        id: popup
        y: dropdownField.height - 1
        width: dropdownField.width
        implicitHeight: contentItem.implicitHeight
        padding: 1
//        visible: dropdownField.activeFocus && dropdownField.model != null && !dropdownField.model.busy && popup.contentItem.count > 0

        contentItem: ListView {
            id: listView
            onCountChanged: dropdownField.togglePopupDisplay();
            clip: true
            implicitHeight: contentHeight
            model: dropdownField.model
            delegate: FluidControls.ListItem {
                width: popup.width
                text: dropdownField.model != null && index > -1 ?
                          dropdownField.model.get(index)[dropdownField.textRole] : ""
                subText: dropdownField.model != null && index > -1 ?
                            dropdownField.model.get(index)[dropdownField.subTextRole] : ""

                onClicked: dropdownField.itemSelected(dropdownField.model.get(index));
            }

            QQC2.ScrollIndicator.vertical: QQC2.ScrollIndicator { }
        }
    }
}
