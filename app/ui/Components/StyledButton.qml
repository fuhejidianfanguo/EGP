import QtQuick
import QtQuick.Controls.Basic
import EGP.ui

Button {
    id: control

    property color buttonColor: Theme.surface
    property color pressedColor: Theme.controlPressed
    property color buttonBorderColor: Theme.border
    property color textColor: Theme.text

    implicitWidth: 30
    implicitHeight: 30
    padding: 0

    background: Rectangle {
        anchors.fill: parent
        color: control.down ? control.pressedColor : control.buttonColor
        border.color: control.buttonBorderColor
        radius: 4
    }

    contentItem: Text {
        text: control.text
        color: control.textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}
