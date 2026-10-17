import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import EGP.ui
import "../../Components"

Rectangle {
    id: root
    color: Theme.sidebar
    objectName: "Settings"
    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            Label {
                text: qsTr("设置")
                color: Theme.text
                font.bold: true
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                Layout.leftMargin: 8
            }
            TextButton {
                text: qsTr("重置")
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                Layout.rightMargin: 8
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            Label {
                color: Theme.text
                text: qsTr("语言")
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                Layout.leftMargin: 8
            }
        }
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
