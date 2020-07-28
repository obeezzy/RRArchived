import "../rrui" as RRUi
import Fluid.Controls 1.0 as FluidControls
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts

Item {
    id: homeCard

    property string title: ""
    default property list<HomeRow> rows

    height: titleLabel.height + card.height

    FluidControls.SubheadingLabel {
        id: titleLabel

        leftPadding: 8
        rightPadding: 8
        text: homeCard.title

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

    }

    RRUi.Card {
        id: card

        height: listView.height

        anchors {
            top: titleLabel.bottom
            left: parent.left
            right: parent.right
        }

        ListView {
            id: listView

            interactive: false
            height: contentItem.childrenRect.height
            model: homeCard.rows !== null ? homeCard.rows.length : null

            anchors {
                left: parent.left
                right: parent.right
            }

            delegate: QQC2.Pane {
                padding: 0
                leftPadding: 8
                rightPadding: 8
                height: 60

                anchors {
                    left: parent.left
                    right: parent.right
                }

                QQLayouts.RowLayout {
                    id: layout

                    anchors.fill: parent

                    Column {
                        spacing: 0
                        QQLayouts.Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                        QQLayouts.Layout.fillWidth: true

                        FluidControls.SubheadingLabel {
                            text: homeCard.rows[index].title

                            anchors {
                                left: parent.left
                                right: parent.right
                            }

                        }

                        FluidControls.BodyLabel {
                            text: rows[index].subtitle
                            color: Material.color(Material.Grey)
                            visible: text.trim().length > 0

                            anchors {
                                left: parent.left
                                right: parent.right
                            }

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

                FluidControls.ThinDivider {
                    anchors.bottom: parent.bottom
                    visible: index !== listView.count - 1
                }

                background: FluidControls.Ripple {
                    anchors.fill: parent
                    onClicked: homeCard.rows[index].clicked()
                }

            }

        }

    }

}
