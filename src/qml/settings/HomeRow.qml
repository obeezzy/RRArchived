import QtQuick 2.12

Item {
    property url iconUrl: ""
    property string title: ""
    property string subtitle: ""
    property Item control: null

    signal clicked()
}
