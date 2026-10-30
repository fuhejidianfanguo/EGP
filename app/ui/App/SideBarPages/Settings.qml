import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import EGP.components

Rectangle {
    id: root
    color: Theme.sidebar
    objectName: "Settings"
    Item {
        anchors.fill: parent
        ColumnLayout {
            anchors.fill: parent
            anchors.leftMargin: 4
            anchors.rightMargin: 4
            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                Label {
                    text: qsTr("设置")
                    color: Theme.text
                    font.bold: true
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                }
                TextButton {
                    text: qsTr("重置")
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                }
            }
            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                Label {
                    color: Theme.text
                    text: qsTr("语言")
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                }
            }
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
