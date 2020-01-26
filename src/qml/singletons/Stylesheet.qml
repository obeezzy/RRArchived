pragma Singleton
import QtQuick 2.12

Item {
    id: stylesheet

    readonly property color creditorYellow: "#b68c29"
    readonly property color debtorRed: "#c0392b"
    readonly property color tableViewDivider: "lightgray"
    // Green - #27ae60

    readonly property color black: "black"
    readonly property color white: "white"
    readonly property color errorRed: "red"

    function padText(text) {
        return text + "                                                    ";
    }
}
