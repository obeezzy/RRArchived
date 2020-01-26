pragma Singleton
import QtQuick 2.12

Item {
    readonly property string localeName: "en_NG"
    readonly property string currencyLocaleName: "en_NG"
    readonly property int shortToastDuration: 4000
    readonly property int _DAYS_IN_A_WEEK: 7

    function toCurrencyString(value) {
        return Number(value).toLocaleCurrencyString(Qt.locale(currencyLocaleName));
    }

    /*
function phoneFormatter() {
  $('.phone').on('input', function() {
    var number = $(this).val().replace(/[^\d]/g, '')
    if (number.length == 7) {
      number = number.replace(/(\d{3})(\d{4})/, "$1-$2");
    } else if (number.length == 10) {
      number = number.replace(/(\d{3})(\d{3})(\d{4})/, "($1) $2-$3");
    }
    $(this).val(number)
  });
};
*/
}
