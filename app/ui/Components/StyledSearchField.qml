import QtQuick
import QtQuick.Controls.Basic
import EGP.ui

TextField {
    id: control

    property bool highlighted: activeFocus || hovered

    implicitWidth: 240
    implicitHeight: 31
    leftPadding: 14
    rightPadding: 14
    topPadding: 8
    bottomPadding: 8
    hoverEnabled: true
    placeholderText: qsTr("Search")
    selectByMouse: true
    color: Theme.text
    placeholderTextColor: Theme.secondary
    selectionColor: Theme.primary
    selectedTextColor: Theme.surface

    Behavior on leftPadding {
        NumberAnimation {
            duration: 180
            easing.type: Easing.OutCubic
        }
    }

    background: Item {
        property real cornerRadius: height / 2

        Rectangle {
            anchors.fill: parent
            radius: parent.cornerRadius
            color: control.highlighted ? Theme.surfacePressed : Theme.surface
            border.color: control.activeFocus ? Theme.primary : Theme.border
            border.width: control.activeFocus ? 2 : 1

            Behavior on color {
                ColorAnimation {
                    duration: 160
                }
            }

            Behavior on border.color {
                ColorAnimation {
                    duration: 180
                }
            }

            Behavior on border.width {
                NumberAnimation {
                    duration: 180
                    easing.type: Easing.OutCubic
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            anchors.margins: -3
            radius: parent.cornerRadius
            color: "transparent"
            border.color: Theme.primary
            border.width: 3
            opacity: control.activeFocus ? 0.18 : 0
            scale: control.activeFocus ? 1 : 0.94

            Behavior on opacity {
                NumberAnimation {
                    duration: 220
                }
            }

            Behavior on scale {
                NumberAnimation {
                    duration: 260
                    easing.type: Easing.OutBack
                }
            }
        }
    }
}
