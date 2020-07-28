import QtQuick 2.12
pragma Singleton

Item {
    // Green - #27ae60

    id: stylesheet

    readonly property color creditorYellow: "#b68c29"
    readonly property color debtorRed: "#c0392b"
    readonly property color paidFullGreen: "green"
    readonly property color tableViewDivider: "lightgray"
    readonly property color black: "black"
    readonly property color white: "white"
    readonly property color errorRed: "red"

    function padText(text) {
        return text + "                                                    ";
    }

}
