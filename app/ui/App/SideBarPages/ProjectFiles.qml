import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import EGP.ui

Rectangle {
    id: root
    objectName: "ProjectFiles"
    color: Theme.sidebar
    Item {
        anchors.fill: parent
        ColumnLayout {
            anchors.fill: parent
            anchors.leftMargin: 4
            anchors.rightMargin: 4
            RowLayout {
                spacing: 2
                Layout.fillWidth: true
                Layout.preferredHeight: 30

                Label {
                    text: qsTr("项目文件")
                    color: Theme.text
                    font.bold: true
                }
                Item {
                    Layout.fillWidth: true
                }
                IconButton {
                    text: "\uf021" // 🔁
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                }
                IconButton {
                    text: "\uf07c" // 📂
                }
            }
            StyledSearchField {
                id: searchField
                Layout.fillWidth: true
                Layout.preferredHeight: 31

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    onPressed: mouse => {
                        const point = searchField.mapToItem(root, mouse.x, mouse.y);
                        editContextMenu.popupFor(searchField, point.x, point.y);
                    }
                }
            }
            EditContextMenu {
                id: editContextMenu
            }
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
