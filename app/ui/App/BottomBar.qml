import QtQuick
import QtQuick.Layouts
import EGP.components

Rectangle {
    id: root

    readonly property int currentFrame: Math.round(slider.value)
    readonly property real currentValue: slider.value
    color: Theme.secondary

    RowLayout {
        anchors.fill: parent

        Item {
            Layout.minimumWidth: 5
        }

        IconButton {
            property bool playing: false
            text: playing ? "\uf04c" : "\uf04b"
            iconColor: playing ? Theme.text : Theme.danger
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
                }
            }
        }

        Item {
            Layout.minimumWidth: 5
        }
    }
}
