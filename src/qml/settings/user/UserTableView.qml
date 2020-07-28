import "../../rrui" as RRUi
import "../../singletons"
import Fluid.Controls 1.0 as FluidControls
import Qt.labs.qmlmodels 1.0 as QQModels
import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3 as QQLayouts
import com.gecko.rr.models 1.0 as RRModels

RRUi.DataTableView {
    id: userTableView

    property Component buttonRow: null
    property int keys: RRModels.UserModel.All
    property string filterText: ""
    property int filterColumn: -1

    signal success(var result)
    signal error(var result)

    function removeUser(userName) {
        userModel.removeUser(userName);
    }

    function refresh() {
        userModel.refresh();
    }

    function undoLastCommit() {
        userModel.undoLastCommit();
    }

    bottomMargin: 20
    clip: true
    visible: !userModel.busy
    flickableDirection: TableView.VerticalFlick
    columnSpacing: 8

    FluidControls.Placeholder {
        visible: userTableView.rows === 0 && !userModel.busy
        anchors.centerIn: parent
        icon.source: FluidControls.Utils.iconUrl("social/person")
        text: qsTr("No registered users.")
    }

    QQC2.BusyIndicator {
        anchors.centerIn: parent
        visible: userModel.busy
    }

    model: RRModels.UserModel {
        id: userModel

        tableViewWidth: userTableView.widthWithoutMargins
        filterText: userTableView.filterText
        filterColumn: userTableView.filterColumn
        keys: userTableView.keys
        onSuccess: userTableView.success(result.code)
        onError: userTableView.error(result.code)
    }

    QQC2.ScrollBar.vertical: RRUi.ScrollBar {
        policy: QQC2.ScrollBar.AlwaysOn
        visible: userTableView.contentHeight > userTableView.height
    }

    delegate: QQModels.DelegateChooser {
        QQModels.DelegateChoice {
            column: RRModels.UserModel.UserColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: userTableView.columnHeader.children[column].width
                implicitHeight: userTableView.columnHeader.children[row].height

                FluidControls.SubheadingLabel {
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: user

                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                }

            }

        }

        QQModels.DelegateChoice {
            column: RRModels.UserModel.ActiveColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: userTableView.columnHeader.children[column].width
                implicitHeight: userTableView.columnHeader.children[row].height

                RRUi.Switch {
                    anchors.verticalCenter: parent.verticalCenter
                    padding: 0
                    checked: active
                    onToggled: userModel.activateUser(user, checked)
                }

            }

        }

        QQModels.DelegateChoice {
            column: RRModels.UserModel.PresetColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: userTableView.columnHeader.children[column].width
                implicitHeight: userTableView.columnHeader.children[row].height

                FluidControls.SubheadingLabel {
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    text: preset

                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }

                }

            }

        }

        QQModels.DelegateChoice {
            column: RRModels.UserModel.ActionColumn

            delegate: RRUi.TableDelegate {
                implicitWidth: userTableView.columnHeader.children[column].width
                implicitHeight: userTableView.columnHeader.children[row].height

                Loader {
                    readonly property var modelData: {
                        "user_id": model.user_id,
                        "user": model.user
                    }

                    anchors.centerIn: parent
                    sourceComponent: userTableView.buttonRow
                }

            }

        }

    }

}
