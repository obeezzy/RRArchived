import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3 as QQLayouts
import QtCharts 2.3 as QCharts
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import "../../rrui" as RRUi
import "../../singletons"

RRUi.HomeCard {
    id: totalRevenueCard

    cardTitle: qsTr("Total revenue made for the week")
    subtitle: qsTr("Revenue for week")

    QQLayouts.RowLayout {
        height: 300
        anchors {
            left: parent.left
            right: parent.right
        }

        QCharts.ChartView {
            id: chartView
            QQLayouts.Layout.preferredWidth: 250
            QQLayouts.Layout.preferredHeight: 250
            legend.alignment: Qt.AlignTop
            legend.visible: false
            antialiasing: true
            animationOptions: QCharts.ChartView.SeriesAnimations
            margins { top: 0; bottom: 0; left: 0; right: 0 }

            QCharts.LineSeries {
                id: lineSeries
                useOpenGL: true
                axisX: QCharts.DateTimeAxis {
                    format: "MMM d"
                    labelsAngle: -45
                    labelsFont {
                        pixelSize: 12
                    }
                    tickCount: 3
                }

                axisY: QCharts.ValueAxis {
                    labelFormat: "%d"
                    titleText: qsTr("Revenue (%1)").arg(Qt.locale(GlobalSettings.localeName).currencySymbol())
                    labelsFont {
                        pixelSize: 12
                    }
                    tickCount: 3
                }

                QCharts.VXYModelMapper {
                    model: totalRevenueCard.model
                    xColumn: 0
                    yColumn: 1
                }
            }
        }

        RRUi.ChartTableView {
            QQLayouts.Layout.fillWidth: true
            QQLayouts.Layout.alignment: Qt.AlignVCenter
            height: contentHeight
            model: totalRevenueCard.model
            interactive: false
            headerData: [
                { "title": qsTr("Date"), "width": 130, "role": "created", "type": "date", "format": "ddd, MMM d" },
                { "title": qsTr("Revenue"), "width": 100, "horizontalAlignment": Qt.AlignHCenter, "role": "amount_paid", "type": "money" }
            ]
        }
    }
}
