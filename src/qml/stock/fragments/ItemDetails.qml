import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import "../../rrui" as RRUi
import com.gecko.rr.models 1.0 as RRModels
import QtQuick.Controls.Material 2.3

Item {
    id: itemDetails

    property int itemId: -1

    readonly property bool dirty: privateProperties.dirty
    readonly property url imageUrl: privateProperties.imageUrl
    readonly property string category: privateProperties.newlyEnteredCategory ? privateProperties.newlyEnteredCategory
                                                                              : categoryComboBox.currentText
    readonly property string item: itemTextField.text
    readonly property string description: descriptionTextField.text
    readonly property double quantity: quantitySpinBox.value
    readonly property string unit: unitTextField.text
    readonly property real costPrice: retailPriceSpinBox.value
    readonly property real retailPrice: retailPriceSpinBox.value
    readonly property bool tracked: trackedCheckBox.checked
    readonly property bool divisible: divisibleCheckBox.checked
    readonly property string categoryNote: ""
    readonly property string itemNote: ""

    QtObject {
        id: privateProperties

        readonly property string defaultCategoryText: qsTr("No category added")
        readonly property string categoryPlaceholderText: qsTr("<Select category>")
        property url imageUrl: ""
        property string newlyEnteredCategory: ""
        property bool dirty: false
    }

    implicitWidth: 300
    implicitHeight: column.height

    Column {
        id: column
        anchors {
            left: parent.left
            right: parent.right
        }
        spacing: 16

        RRUi.SectionFragment {
            id: categoryFragment
            title: qsTr("Category")
            anchors {
                left: parent.left
                right: parent.right
            }

            Row {
                FluidControls.Icon {
                    anchors.verticalCenter: parent.verticalCenter
                    source: Qt.resolvedUrl("qrc:/icons/shape.svg")
                }

                Item { width: 12; height: 1 }

                QQC2.ComboBox {
                    id: categoryComboBox
                    visible: !categoryLabel.visible
                    enabled: visible
                    focus: enabled
                    width: 220
                    textRole: "category"
                    currentIndex: -1
                    displayText: currentIndex > -1 ? currentText : privateProperties.categoryPlaceholderText
                    model: RRModels.StockCategoryModel {
                        onSuccess: categoryComboBox.currentIndex = categoryComboBox.find(stockItemDetailRecord.category);
                    }
                    onCurrentIndexChanged: privateProperties.dirty = true;
                }

                QQC2.Label {
                    id: categoryLabel
                    anchors.verticalCenter: parent.verticalCenter
                    width: 200
                    font.pixelSize: 17
                    color: privateProperties.newlyEnteredCategory === "" ? Material.color(Material.Grey)
                                                                         : Material.theme === Material.Dark ? "white"
                                                                                                            : "black"
                    text: privateProperties.newlyEnteredCategory === "" ? privateProperties.defaultCategoryText
                                                                        : privateProperties.newlyEnteredCategory
                    visible: privateProperties.newlyEnteredCategory || !categoryComboBox.count
                }

                Item { width: 4; height: 1 }

                RRUi.ToolButton {
                    id: addCategoryButton
                    anchors.verticalCenter: parent.verticalCenter
                    icon.source: FluidControls.Utils.iconUrl(privateProperties.newlyEnteredCategory ? "content/remove_circle"
                                                                                                    : "content/add_circle")
                    text: privateProperties.newlyEnteredCategory ? qsTr("Remove category")
                                                                 : qsTr("Add category")

                    onClicked: {
                        if (privateProperties.newlyEnteredCategory)
                            privateProperties.newlyEnteredCategory = "";
                        else
                            categoryInputDialog.open();

                        privateProperties.dirty = true;
                    }
                }

                RRUi.ToolButton {
                    id: searchButton
                    anchors.verticalCenter: parent.verticalCenter
                    icon.source: FluidControls.Utils.iconUrl("action/search")
                    text: qsTr("Search")
                }
            }
        }

        RRUi.SectionFragment {
            id: itemFragment
            title: qsTr("Product")
            anchors {
                left: parent.left
                right: parent.right
            }

            RRUi.IconTextField {
                id: itemTextField
                icon.source: Qt.resolvedUrl("qrc:/icons/shopping.svg")
                textField.placeholderText: qsTr("Product name");
                textField.onTextEdited: privateProperties.dirty = true;
            }

            RRUi.IconTextField {
                id: descriptionTextField
                icon.source: Qt.resolvedUrl("qrc:/icons/book-open-variant.svg")
                textField.placeholderText: qsTr("Short description");
                textField.onTextEdited: privateProperties.dirty = true;
            }
        }

        RRUi.SectionFragment {
            id: quantityUnitFragment
            title: qsTr("Quantity and unit")
            anchors {
                left: parent.left
                right: parent.right
            }

            Row {
                spacing: 12

                FluidControls.Icon {
                    anchors.verticalCenter: parent.verticalCenter
                    source: Qt.resolvedUrl("qrc:/icons/cube-outline.svg")
                }

                QQC2.SpinBox {
                    id: quantitySpinBox
                    down.indicator: null
                    up.indicator: null
                    width: 50
                    editable: itemDetails.itemId <= 0
                    to: 10000
                    onValueChanged: privateProperties.dirty = true;
                }

                RRUi.TextField {
                    id: unitTextField
                    placeholderText: qsTr("Unit")
                    width: 200
                    onTextEdited: privateProperties.dirty = true;
                }
            }

            Row {
                spacing: 12

                QQC2.CheckBox {
                    id: trackedCheckBox
                    text: qsTr("Can be tracked")
                    onToggled: privateProperties.dirty = true;
                }

                QQC2.CheckBox {
                    id: divisibleCheckBox
                    text: qsTr("Can be divided")
                    checked: true
                    onToggled: privateProperties.dirty = true;
                }
            }
        }

        RRUi.SectionFragment {
            id: priceFragment
            title: qsTr("Price")
            anchors {
                left: parent.left
                right: parent.right
            }

            Row {
                spacing: 12

                FluidControls.Icon {
                    anchors.verticalCenter: parent.verticalCenter
                    source: Qt.resolvedUrl("qrc:/icons/tag.svg")
                }

                QQC2.SpinBox {
                    id: retailPriceSpinBox
                    down.indicator: null
                    up.indicator: null
                    width: 50
                    editable: true
                    to: 10000000
                    onValueChanged: privateProperties.dirty = true;

                    property int decimals: 2
                    property real realValue: value / 100

                    validator: DoubleValidator {
                        bottom: Math.min(retailPriceSpinBox.from, retailPriceSpinBox.to)
                        top:  Math.max(retailPriceSpinBox.from, retailPriceSpinBox.to)
                    }

                    textFromValue: function(value, locale) {
                        return "\u20a6 " + Number(value).toLocaleString(locale, 'f', retailPriceSpinBox.decimals)
                    }

                    valueFromText: function(text, locale) {
                        return Number.fromLocaleString(locale, text.replace("\u20a6 ", ""))
                    }
                }
            }
        }
    }

    RRUi.InputDialog {
        id: categoryInputDialog
        text: qsTr("Enter category name              ")
        textField.placeholderText: qsTr("Category")

        onAboutToShow: textField.text = "";
        onAccepted: {
            if (textField.text.trim().length > 0 && categoryComboBox.find(textField.text.trim()) === -1) {
                privateProperties.newlyEnteredCategory = textField.text;
                categoryComboBox.currentIndex = -1;
            } else if (categoryComboBox.find(textField.text.trim()) > -1) {
                privateProperties.newlyEnteredCategory = "";
                categoryComboBox.currentIndex = categoryComboBox.find(textField.text.trim());
            }
        }
    }

    RRModels.StockItemDetailRecord {
        id: stockItemDetailRecord
        autoQuery: itemDetails.itemId > 0
        itemId: itemDetails.itemId

        onSuccess: {
            privateProperties.imageUrl = imageUrl;
            categoryComboBox.currentIndex = categoryComboBox.find(category);
            itemTextField.text = item;
            descriptionTextField.text = description;
            quantitySpinBox.value = quantity;
            unitTextField.text = unit;
            retailPriceSpinBox.value = retailPrice;
            trackedCheckBox.checked = true;
            divisibleCheckBox.checked = divisible;
        }
    }
}
