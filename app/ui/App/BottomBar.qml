import QtQuick
import QtQuick.Layouts
import "../Components"

Rectangle {
    id: root

    readonly property int currentFrame: Math.round(slider.value)
    readonly property real currentValue: slider.value
    color: '#797979'

    RowLayout {
        anchors.fill: parent

        Item {
            Layout.minimumWidth: 5
        }

        IconButton {
            property bool playing: false
            text: playing ? "\uf04c" : "\uf04b"
            iconColor: playing ? "black" : "red"
            onClicked: {
                playing = !playing;
            }
        }

        StyledSlider {
            id: slider
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            Layout.alignment: Qt.AlignVCenter

            from: 0
            to: 100

            onPressedChanged: {
                if (!pressed) {
                    value = Math.round(value);
                    console.log("slider value changed:", value);
                }
            }
        }

        Item {
            Layout.minimumWidth: 5
        }
    }
}
