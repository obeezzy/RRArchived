import QtQuick 2.10
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Layouts 1.3 as QQLayouts
import com.gecko.rr.widgets 1.0 as RRWidgets
import "."

Dialog {
    id: wizard

    property Component buttonRow: null
    property alias stackView: stackView
    property alias backButton: backButton
    signal finished(var transactionInfo)

    title: ""
    standardButtons: Dialog.NoButton
    width: 600
    height: 540
    closePolicy: QQC2.Popup.CloseOnEscape

    contentItem: QQLayouts.ColumnLayout {
        Row {
            QQLayouts.Layout.preferredHeight: 25

            ToolButton {
                id: backButton
                anchors.verticalCenter: parent.verticalCenter
                width: FluidControls.Units.iconSizes.large
                height: width
                icon.source: FluidControls.Utils.iconUrl("navigation/arrow_back")
                visible: stackView.depth > 1
                text: qsTr("Back")
                onClicked: stackView.pop();
            }

            FluidControls.TitleLabel {
                id: dialogLabel
                anchors.verticalCenter: parent.verticalCenter

                wrapMode: Text.Wrap
                text: stackView.currentItem !== null ? stackView.currentItem.title : ""
            }
        }

        QQC2.StackView {
            id: stackView

            property string currentObjectName: currentItem != null ? currentItem.objectName : ""

            QQLayouts.Layout.fillWidth: true
            QQLayouts.Layout.fillHeight: true
            clip: true
        }

        Loader {
            id: buttonRowLoader
            sourceComponent: wizard.buttonRow
            QQLayouts.Layout.alignment: Qt.AlignRight
        }
    }
}
