import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr.models 1.0 as RRModels
import "../../rrui" as RRUi

ListView {
    id: userPrivilegeListView

    property int userId: -1
    property url imageUrl: ""
    property string firstName: ""
    property string lastName: ""
    property string userName: ""
    property string password: ""
    property string phoneNumber: ""
    property string emailAddress: ""

    signal success(int successCode)
    signal error(int errorCode)

    function submit() { return userPrivilegeModel.submit(); }

    clip: true
    spacing: 24
    topMargin: 24
    bottomMargin: 24

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        visible: userPrivilegeListView.count > 0
        policy: QQC2.ScrollBar.AlwaysOn
    }

    model: RRModels.UserPrivilegeModel {
        id: userPrivilegeModel
        userId: userPrivilegeListView.userId
        imageUrl: userPrivilegeListView.imageUrl
        firstName: userPrivilegeListView.firstName
        lastName: userPrivilegeListView.lastName
        userName: userPrivilegeListView.userName
        password: userPrivilegeListView.password
        phoneNumber: userPrivilegeListView.phoneNumber
        emailAddress: userPrivilegeListView.emailAddress

        onSuccess: userPrivilegeListView.success(successCode);
        onError: userPrivilegeListView.error(errorCode);
    }
    delegate: RRUi.Card {
        id: card
        readonly property int groupIndex: index

        width: ListView.view.width
        height: column.height + divider.height + 24
        padding: 8
        bottomPadding: 0
        Material.elevation: 0

        Column {
            id: column
            anchors {
                left: parent.left
                right: parent.right
            }
            spacing: 24

            FluidControls.HeadlineLabel {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                text: title
            }

            ListView {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                spacing: 32
                height: contentItem.childrenRect.height
                interactive: false
                model: privilege_model
                delegate: FluidControls.ListItem {
                    width: ListView.view.width
                    height: Math.max(60, layout.height)
                    padding: 16
                    bottomPadding: 24

                    QQLayouts.RowLayout {
                        id: layout
                        anchors {
                            left: parent.left
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                        }
                        spacing: 64
                        height: Math.max(descriptionColumn.height, valueSwitch.height)

                        Column {
                            id: descriptionColumn
                            QQLayouts.Layout.fillWidth: true
                            QQLayouts.Layout.alignment: Qt.AlignVCenter

                            FluidControls.SubheadingLabel {
                                visible: short_description !== ""
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }

                                text: short_description
                            }

                            FluidControls.SubheadingLabel {
                                visible: long_description !== ""
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }

                                text: long_description
                                wrapMode: Text.WordWrap
                                maximumLineCount: 4
                                color: Material.color(Material.Grey)
                            }
                        }

                        RRUi.Switch {
                            id: valueSwitch
                            QQLayouts.Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                            checked: value
                            onToggled: value = checked;
                        }
                    }
                }
            }
        }

        FluidControls.ThinDivider {
            id: divider
            anchors.bottom: parent.bottom
            visible: index !== userPrivilegeListView.count - 1
        }
    }
}
