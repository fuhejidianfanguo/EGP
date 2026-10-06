pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../Components"

// 只负责应用骨架布局，具体内容交给各子组件。
ColumnLayout {
    id: root

    spacing: 0
    anchors.fill: parent

    SystemPalette {
        id: sysPalette
        colorGroup: SystemPalette.Active
    }

    SplitView {
        Layout.fillWidth: true
        Layout.fillHeight: true
        orientation: Qt.Horizontal
        handle: SplitHandle {
            orientation: Qt.Horizontal
        }

        SideBar {
            id: sideBar
            SplitView.fillHeight: true
            SplitView.preferredWidth: 170
            SplitView.maximumWidth: 300
            SplitView.minimumWidth: 130
        }

        SplitView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            orientation: Qt.Vertical
            handle: SplitHandle {
                orientation: Qt.Vertical
            }

            Scene {
                id: scene
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                SplitView.minimumHeight: 170
            }

            BottomBar {
                id: bottomBar
                SplitView.fillWidth: true
                SplitView.preferredHeight: 60
                SplitView.minimumHeight: 50
            }
        }
    }

    AppStatusBar {
        Layout.fillWidth: true
        currentFrame: bottomBar.currentFrame
        onSideBarToggleRequested: sideBar.visible = !sideBar.visible
    }
}
