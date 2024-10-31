import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import WebRTC

Window {
    width: 1010
    height: 520
    visible: true
    title: qsTr("CA1")

    WebRTC {
        id: rtc
    }

    Item {
        anchors.fill: parent

        // Scrollable debug panel on the left
        ScrollView {
            id: scrollView
            width: parent.width * 0.5  // Adjusts width to 40% of the window
            height: parent.height
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            clip: true

            TextArea {
                id: logOutput
                width: parent.width
                height: parent.height
                readOnly: true
                text: ""
            }
        }

        ColumnLayout {
            anchors {
                top: parent.top
                left: scrollView.right
                right: parent.right
                bottom: callbtn.top
                margins: 20
            }

            Label {
                text: "Ip: " + "172.16.142.176"
                Layout.fillWidth: true
                Layout.preferredHeight: 40
            }
            Label {
                text: "IceCandidate: " + "172.16.142.176"
                Layout.fillWidth: true
                Layout.preferredHeight: 40
            }
            Label {
                text: "CallerId: " + textfield.text
                Layout.fillWidth: true
                Layout.preferredHeight: 40
            }

            TextField {
                id: textfield
                placeholderText: "Phone Number"
                Layout.fillWidth: true
                enabled: !callbtn.pushed
            }
        }

        Connections {
            target: rtc
            onDebugMessage: logOutput.text += message + "\n"
        }

        Button {
            id: callbtn
            property bool pushed: false
            height: 47
            text: "Call"
            Material.background: "green"
            Material.foreground: "white"
            anchors {
                bottom: parent.bottom
                left: scrollView.right
                right: parent.right
                margins: 20
            }

            onClicked: {
                pushed = !pushed
                if (pushed) {
                    Material.background = "red"
                    text = "End Call"
                    rtc.setId(textfield.text)
                    rtc.init()
                    rtc.createOffer()
                } else {
                    Material.background = "green"
                    text = "Call"
                    textfield.clear()
                }
            }
        }
    }
}
