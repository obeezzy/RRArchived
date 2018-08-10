pragma Singleton
import QtQuick 2.10

Item {
    readonly property string localeName: "en_NG"
    readonly property string currencyLocaleName: "en_NG"
    readonly property int shortToastDuration: 4000

    function toCurrencyString(value) {
        return Number(value).toLocaleCurrencyString(Qt.locale(currencyLocaleName));
    }
}
