import QtQuick
import QtQuick.Controls.Basic

// 图标按钮：字体的 family/styleName 由 FaIcon 统一处理，调用方无需再手动传递。
Button {
    id: control

    property int iconSize: 16
    property string iconColor: Theme.text
    property bool solid: true

    implicitWidth: 30
    implicitHeight: 30
    padding: 0

    background: Rectangle {
        anchors.fill: parent
        color: control.down ? Theme.controlPressed : Theme.surface
        border.color: Theme.border
        radius: 4
    }

    contentItem: FaIcon {
        text: control.text
        solid: control.solid
        color: control.iconColor
        font.pixelSize: control.iconSize

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
