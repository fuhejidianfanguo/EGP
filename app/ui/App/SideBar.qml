import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "SideBarPages"

Rectangle {
    id: root

    Layout.fillHeight: true
    Layout.preferredWidth: 150
    color: Theme.sidebar

    function showPage(pageUrl) {
        pageStack.replace(pageUrl)
    }

    function showSettings() {
        showPage(Qt.resolvedUrl("SideBarPages/Settings.qml"))
    }

    function showProjectFiles() {
        showPage(Qt.resolvedUrl("SideBarPages/ProjectFiles.qml"))
    }

    StackView {
        id: pageStack
        anchors.fill: parent
        clip: true
        initialItem: projectFilesPage

        Component {
            id: projectFilesPage
            ProjectFiles {}
        }

        pushEnter: Transition {
            ParallelAnimation {
                NumberAnimation {
                    property: "x"
                    from: pageStack.width
                    to: 0
                    duration: 220
                    easing.type: Easing.OutCubic
                }
                NumberAnimation {
                    property: "opacity"
                    from: 0
                    to: 1
                    duration: 180
                }
            }
        }

        pushExit: Transition {
            NumberAnimation {
                property: "x"
                to: -pageStack.width * 0.25
                duration: 220
                easing.type: Easing.OutCubic
            }
        }

        popEnter: Transition {
            NumberAnimation {
                property: "x"
                from: -pageStack.width * 0.25
                to: 0
                duration: 220
                easing.type: Easing.OutCubic
            }
        }

        popExit: Transition {
            ParallelAnimation {
                NumberAnimation {
                    property: "x"
                    to: pageStack.width
                    duration: 220
                    easing.type: Easing.OutCubic
                }
                NumberAnimation {
                    property: "opacity"
                    to: 0
                    duration: 180
                }
            }
        }

        replaceEnter: Transition {
            NumberAnimation {
                property: "x"
                from: pageStack.width
                to: 0
                duration: 220
                easing.type: Easing.OutCubic
            }
        }

        replaceExit: Transition {
            NumberAnimation {
                property: "x"
                to: -pageStack.width * 0.25
                duration: 220
                easing.type: Easing.OutCubic
            }
        }
    }
}
