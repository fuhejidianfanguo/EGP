import QtQuick
import QtQuick.Layouts
import "../Components"

// 状态栏的具体内容；通过信号把动作交给 Framework 处理，避免反向依赖具体布局。
StatusBar {
    id: root

    property int currentFrame: 0

    signal sideBarToggleRequested
    signal fileRequested
    signal settingsRequested

    IconButton {
        text: "\uf0c9"
        onClicked: root.sideBarToggleRequested()
    }
    IconButton {
        text: "\uf15b"
        onClicked: root.fileRequested()
    }
    IconButton {
        text: "\uf013"
        onClicked: root.settingsRequested()
    }

    Item {
        Layout.fillWidth: true
    }

    Text {
        Layout.rightMargin: 8
        color: "#333333"
        text: qsTr("帧 ") + root.currentFrame
    }
}
