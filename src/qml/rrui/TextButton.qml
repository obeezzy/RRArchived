import QtQuick 2.12
import QtQuick.Controls 2.12
import "../flatui" as Flat
import "."

Button {
    id: textButton

    property color hoverColor: Flat.Stylesheet.turquoise
    property color color: Flat.Stylesheet.asbestos
    property string toolTipText: ""

    hoverEnabled: true
    font {
        family: Flat.Stylesheet.latoLightFont.name
        pixelSize: 16
        styleName: "Light"
    }

    background: Item {
        implicitWidth: Math.max(textMetrics.width, 50)
        implicitHeight: textMetrics.height

        TextMetrics {
            id: textMetrics
            font.family: textButton.font.family
            text: textButton.text
        }
    }

    contentItem: Text {
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignHCenter
        color: area.containsMouse ? textButton.hoverColor : textButton.color
        scale: area.pressed ? .95 : 1
        font: textButton.font
        width: contentWidth
        height: contentHeight
        text: textButton.text

        MouseArea {
            id: area
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor

            onClicked: {
                textButton.clicked();
                mouse.accepted = false;
            }
        }
    }

    ToolTip {
        text: textButton.toolTipText
        visible: textButton.hovered && textButton.toolTipText.length > 0
    }
}
