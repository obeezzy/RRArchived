import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import QtQuick.Layouts 1.3 as QQLayouts

Dialog {
    id: wizard

    property Component initialPage: null
    signal finished

    x: (QQC2.ApplicationWindow.contentItem.width - width) / 2
    y: (QQC2.ApplicationWindow.contentItem.height - height) / 2
    parent: MainWindow.contentItem
    title: ""
    standardButtons: Dialog.NoButton
    width: 600
    height: 540
    closePolicy: QQC2.Popup.CloseOnEscape

    header: Item {
        implicitHeight: childrenRect.height

        QQLayouts.RowLayout {
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                topMargin: 8
                leftMargin: 8
                rightMargin: 8
            }
            height: backButton.height

            ToolButton {
                id: backButton
                QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                QQLayouts.Layout.preferredWidth: FluidControls.Units.iconSizes.large
                QQLayouts.Layout.preferredHeight: width
                icon.source: FluidControls.Utils.iconUrl("navigation/arrow_back")
                enabled: stackView.currentItem ? stackView.currentItem.previousEnabled : true
                text: qsTr("Back")
                visible: stackView.depth > 1
                onClicked: {
                    if (stackView.currentItem) {
                        stackView.currentItem.previous();
                        stackView.pop();
                    }
                }
            }

            FluidControls.DialogLabel {
                QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                QQLayouts.Layout.fillWidth: true
                QQLayouts.Layout.leftMargin: backButton.visible ? 0 : 16
                wrapMode: Text.WordWrap
                text: stackView.currentItem !== null ? stackView.currentItem.title : ""
            }
        }
    }

    contentItem: StackView {
        id: stackView

        readonly property var wizard: wizard
        signal finished

        clip: true
        initialItem: wizard.initialPage
        onFinished: {
            wizard.finished();
            wizard.accept();
        }
    }

    onAboutToShow: if (stackView.depth === 0 && stackView.initialItem) stackView.push(stackView.initialItem);
    onAboutToHide: stackView.clear();

    footer: Item {
        implicitHeight: childrenRect.height

        QQLayouts.RowLayout {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                leftMargin: 8
                rightMargin: 8
                bottomMargin: 8
            }
            spacing: 8

            Item {
                QQLayouts.Layout.fillWidth: true
                QQLayouts.Layout.fillHeight: true
            }

            QQC2.Button {
                flat: true
                text: qsTr("Cancel")
                onClicked: wizard.close();
            }

            QQC2.Button {
                enabled: stackView.currentItem ? stackView.currentItem.nextEnabled : true
                text: stackView.currentItem ? stackView.currentItem.nextButtonText : ""
                onClicked: {
                    if (stackView.currentItem)
                        stackView.currentItem.next();
                    if (stackView.currentItem && stackView.currentItem.nextEnabled)
                        stackView.push(stackView.currentItem.nextPage.component, stackView.currentItem.nextPage.properties);
                }
            }
        }
    }
}
