import QtQuick 2.10
import com.gecko.rr.models 1.0 as RRModels
import "../singletons"

ListModel {
    id: itemDetailModel
    property int itemId: -1
    readonly property var record: RRModels.StockItemDetailRecord {
        id: stockItemDetailRecord
        itemId: itemDetailModel.itemId
        onSuccess: {
            clear();
            append({ "title": qsTr("Category"), "value": stockItemDetailRecord.category });
            append({ "title": qsTr("Item"), "value": stockItemDetailRecord.item });
            if (stockItemDetailRecord.description !== "")
                append({ "title": qsTr("Description"), "value": stockItemDetailRecord.description });
            append({ "title": qsTr("Quantity"), "value": stockItemDetailRecord.quantity.toString() });
            append({ "title": qsTr("Unit"), "value": stockItemDetailRecord.unit });
            append({ "title": qsTr("Cost price"), "value": stockItemDetailRecord.costPrice.toFixed(2) });
            append({ "title": qsTr("Retail price"), "value": stockItemDetailRecord.retailPrice.toFixed(2) });
            append({ "title": qsTr("Currency"), "value": stockItemDetailRecord.currency });
            append({ "title": qsTr("Created"), "value": Qt.formatDateTime(stockItemDetailRecord.created, "MMM d yyyy, h:mm AP") });
            append({ "title": qsTr("Last edited"), "value": Qt.formatDateTime(stockItemDetailRecord.lastEdited, "MMM d yyyy, h:mm AP") });
            append({ "title": qsTr("User"), "value": stockItemDetailRecord.user });
        }
    }
}
