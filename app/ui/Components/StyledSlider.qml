pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls

Slider {
    id: control

    readonly property int tickDivisions: Math.ceil((to - from) / 10)
    readonly property int tickCount: tickDivisions + 1

    readonly property real handleWidth: control.handle ? control.handle.implicitWidth : 0

    background: Item {
        id: bg

        x: control.leftPadding + control.handleWidth / 2
        y: (control.availableHeight - height) / 2 + 8   // ← 加 8 往下推

        implicitWidth: 200
        implicitHeight: 30
        width: control.availableWidth - control.handleWidth
        height: implicitHeight

        Rectangle {
            id: track
            y: 0
            width: parent.width
            height: 8
            radius: 4
            color: "#ffffff"
            antialiasing: true
        }

        Rectangle {
            width: control.visualPosition * track.width
            height: track.height
            topLeftRadius: 4
            bottomLeftRadius: 4
            color: "#2196F3"
            antialiasing: true
        }

        Repeater {
            model: control.tickCount

            Item {
                id: tickItem
                required property int index

                property real tickValue: control.from + index * ((control.to - control.from) / control.tickDivisions)
                property real ratio: index / control.tickDivisions
                property int labelStep: Math.max(1, Math.ceil(control.tickCount / 10))

                x: ratio * parent.width - width / 2
                y: 0
                width: 20
                height: parent.height

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    y: 12
                    width: 2
                    height: 6
                    color: "#333"
                }

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    y: 20
                    visible: tickItem.index % tickItem.labelStep === 0 || tickItem.index === control.tickCount - 1
                    text: tickItem.tickValue.toFixed(0)
                    font.pixelSize: 10
                    color: "#333"
                }
            }
        }
    }

    handle: Rectangle {
        x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
        y: bg.y + 4 - height / 2   // 跟随 background 的轨道中心
        implicitWidth: 6
        implicitHeight: 26
        radius: 13
        color: control.pressed ? "#f0f0f0" : "#ffffff"
        border.color: "#bdbdbd"
        border.width: 1
    }
}
