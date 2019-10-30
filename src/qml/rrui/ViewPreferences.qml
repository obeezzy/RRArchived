import QtQuick 2.12

QtObject {
    id: viewPreferences

    property int filterIndex: 0
    property int sortColumnIndex: 0
    property int sortOrderIndex: 0

    property var filterModel: []
    property var sortColumnModel: []
    property var sortOrderModel: [
        "Sort in ascending order",
        "Sort in descending order"
    ]

    readonly property string filterColumnOption: filterModel.length > 0 ? filterModel[filterIndex] : ""
    readonly property string sortColumnOption: sortColumnModel.length > 0 ? sortColumnModel[sortColumnIndex] : ""
    readonly property string sortOrderOption: sortOrderModel.length > 0 ? sortOrderModel[sortOrderIndex] : ""

    readonly property var model: [ filterColumnOption, sortColumnOption, sortOrderOption ]
}
