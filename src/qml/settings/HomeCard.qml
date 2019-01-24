import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.3 as QQLayouts
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls

Item {
    id: homeCard

    property string title: ""
    default property list<HomeRow> rows

    height: titleLabel.height + card.height

    FluidControls.SubheadingLabel {
        id: titleLabel
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        leftPadding: 8
        rightPadding: 8
        text: homeCard.title
    }

    FluidControls.Card {
        id: card
        anchors {
            top: titleLabel.bottom
            left: parent.left
            right: parent.right
        }
        height: listView.height

        ListView {
            id: listView
            anchors {
                left: parent.left
                right: parent.right
            }
            interactive: false

            height: contentItem.childrenRect.height
            model: homeCard.rows !== null ? homeCard.rows.length : null

            delegate: QQC2.Pane {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                padding: 0
                leftPadding: 8
                rightPadding: 8
                height: 60

                background: FluidControls.Ripple {
                    anchors.fill: parent
                    onClicked: homeCard.rows[index].clicked();
                }

                QQLayouts.RowLayout {
                    id: layout
                    anchors.fill: parent

                    Column {
                        spacing: 0
                        QQLayouts.Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                        QQLayouts.Layout.fillWidth: true

                        FluidControls.SubheadingLabel {
                            anchors {
                                left: parent.left
                                right: parent.right
                            }

                            text: homeCard.rows[index].title
                        }

                        FluidControls.BodyLabel {
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                            text: rows[index].subtitle
                            color: Material.color(Material.Grey)
                            visible: text.trim().length > 0
                        }
                    }

                    Item {
                        id: controlItem
                        width: childrenRect.width
                        height: childrenRect.height
                        QQLayouts.Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                        Binding {
                            when: homeCard.control !== null
                            target: controlItem
                            property: "children"
                            value: rows[index].control
                        }
                    }

                    FluidControls.Icon {
                        visible: rows[index].control === null
                        source: Qt.resolvedUrl("qrc:/icons/menu-right.svg")
                        QQLayouts.Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    }
                }

                FluidControls.ThinDivider { anchors.bottom: parent.bottom; visible: index !== listView.count - 1 }
            }
        }
    }
}
