import QtQuick
import QtQuick.Controls
import EGP.ui

Menu {
    id: root

    property var target: null
    property int savedStart: 0
    property int savedEnd: 0

    function popupFor(item, x, y) {
        target = item;
        if (target) {
            savedStart = target.selectionStart;
            savedEnd = target.selectionEnd;
        }
        popup(x, y);
    }

    onClosed: {
        if (target) {
            target.select(savedStart, savedEnd);
        }
    }

    background: Rectangle {
        implicitWidth: 160
        color: Theme.surface
        radius: 6
        border.color: Theme.border
        border.width: 1
    }

    component StyledItem: MenuItem {
        id: menuItem
        implicitHeight: 32

        // 改名，避免与 MenuItem 继承来的 final 属性冲突
        property bool itemHovered: false

        HoverHandler {
            onHoveredChanged: menuItem.itemHovered = hovered
        }

        contentItem: Text {
            text: menuItem.text
            color: menuItem.enabled ? Theme.text : Theme.secondary
            font.pixelSize: 14
            leftPadding: 12
            verticalAlignment: Text.AlignVCenter
        }

        background: Rectangle {
            anchors.fill: parent
            anchors.margins: 1
            radius: 4
            color: (menuItem.hovered && menuItem.enabled) ? Theme.surfacePressed : "transparent"
        }
    }

    StyledItem {
        text: qsTr("剪切")
        enabled: root.target !== null && root.target.selectedText.length > 0
        onTriggered: root.target.cut()
    }

    StyledItem {
        text: qsTr("复制")
        enabled: root.target !== null && root.target.selectedText.length > 0
        onTriggered: root.target.copy()
    }

    StyledItem {
        text: qsTr("粘贴")
        enabled: root.target !== null && root.target.canPaste
        onTriggered: root.target.paste()
    }

    MenuSeparator {
        contentItem: Rectangle {
            implicitWidth: 140
            implicitHeight: 1
            color: Theme.border
        }
    }

    StyledItem {
        text: qsTr("全选")
        enabled: root.target !== null && root.target.length > 0
        onTriggered: root.target.selectAll()
    }
}
