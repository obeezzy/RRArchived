import QtQuick 2.13
import Fluid.Controls 1.0 as FluidControls
import Qt.labs.lottieqt 1.0

Item {
    id: checkmark

    property string text: ""
    signal finished
    function animate() { visible = true; animation.gotoAndPlay(animation.startFrame); }

    visible: false
    implicitWidth: column.width
    implicitHeight: column.height

    Column {
        id: column
        anchors.centerIn: parent
        width: animation.width
        spacing: 8

        LottieAnimation {
            id: animation
            loops: 1
            quality: LottieAnimation.MediumQuality
            source: ":/lottie/check_animation.json"
            autoPlay: false

            onStatusChanged: {
                if (status === LottieAnimation.Ready)
                    frameRate = 120;
            }

            onFinished: {
                label.opacity = 1;
                checkmark.finished();
            }
        }

        FluidControls.TitleLabel {
            id: label
            anchors.horizontalCenter: parent.horizontalCenter
            opacity: 0
            text: checkmark.text
            color: "white"

            Behavior on opacity { OpacityAnimator { from: 0; to: 1; easing.type: Easing.OutCubic; duration: 150 } }
        }
    }
}
