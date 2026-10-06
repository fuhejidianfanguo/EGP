import QtQuick
import QtQuick.Layouts

// 状态栏容器：内容由调用方通过默认属性填充，内容区横向铺满。
Rectangle {
    id: root

    Layout.preferredHeight: 30
    Layout.fillWidth: true
    color: "lightgray"

    default property alias content: contentRow.data

    RowLayout {
        id: contentRow
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        spacing: 1
    }
}
