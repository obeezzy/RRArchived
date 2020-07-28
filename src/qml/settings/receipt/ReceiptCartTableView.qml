import Qt.labs.qmlmodels 1.0 as QQModels
import QtQuick 2.12

TableView {
    id: receiptCartTableView

    enum Columns {
        QuantityColumn,
        ProductColumn,
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
            column: ReceiptCartTableView.ProductColumn

            delegate: Text {
                horizontalAlignment: Qt.AlignLeft
                verticalAlignment: Qt.AlignVCenter
                text: product
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
