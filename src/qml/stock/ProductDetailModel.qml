import QtQuick 2.12
import com.gecko.rr.models 1.0 as RRModels
import "../singletons"

ListModel {
    id: productDetailModel
    property int productId: -1
    readonly property var record: RRModels.StockProductDetailRecord {
        id: stockProductDetailRecord
        productId: productDetailModel.productId
        onSuccess: {
            clear();
            append({ "title": qsTr("Category"), "value": stockProductDetailRecord.category });
            append({ "title": qsTr("Product"), "value": stockProductDetailRecord.product });
            if (stockProductDetailRecord.description !== "")
                append({ "title": qsTr("Description"), "value": stockProductDetailRecord.description });
            append({ "title": qsTr("Quantity"), "value": stockProductDetailRecord.quantity.toString() });
            append({ "title": qsTr("Unit"), "value": stockProductDetailRecord.unit });
            append({ "title": qsTr("Cost price"), "value": stockProductDetailRecord.costPrice.toFixed(2) });
            append({ "title": qsTr("Retail price"), "value": stockProductDetailRecord.retailPrice.toFixed(2) });
            append({ "title": qsTr("Currency"), "value": stockProductDetailRecord.currency });
            append({ "title": qsTr("Created"), "value": Qt.formatDateTime(stockProductDetailRecord.created, "MMM d yyyy, h:mm AP") });
            append({ "title": qsTr("Last edited"), "value": Qt.formatDateTime(stockProductDetailRecord.lastEdited, "MMM d yyyy, h:mm AP") });
            append({ "title": qsTr("User"), "value": stockProductDetailRecord.user });
        }
    }
}
