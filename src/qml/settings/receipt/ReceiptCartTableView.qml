import QtQuick 2.12
import Qt.labs.qmlmodels 1.0 as QQModels

TableView {
    id: receiptCartTableView

    enum Columns {
        QuantityColumn,
        ItemColumn,
        PriceColumn,
        TotalColumn
    }

    implicitWidth: 400
    height: contentItem.childrenRect.height

    delegate: QQModels.DelegateChooser {
        QQModels.DelegateChoice {
            column: ReceiptCartTableView.QuantityColumn
            delegate: Text {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: quantity
            }
        }

        QQModels.DelegateChoice {
            column: ReceiptCartTableView.ItemColumn
            delegate: Text {
                horizontalAlignment: Qt.AlignLeft
                verticalAlignment: Qt.AlignVCenter
                text: item
            }
        }

        QQModels.DelegateChoice {
            column: ReceiptCartTableView.PriceColumn
            delegate: Text {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: Number(price).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
            }
        }

        QQModels.DelegateChoice {
            column: ReceiptCartTableView.TotalColumn
            delegate: Text {
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                text: Number(total).toLocaleCurrencyString(Qt.locale(GlobalSettings.currencyLocaleName))
            }
        }
    }
}
