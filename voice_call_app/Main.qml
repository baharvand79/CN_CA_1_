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

        ScrollView {
            id: scrollView
            width: parent.width * 0.5
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

            // Row for Caller ID
            RowLayout {
                spacing: 10

                TextField {
                    id: textfield_callerID
                    placeholderText: "Caller ID"
                    Layout.fillWidth: true
                    enabled: !button_callerID.pushed
                }

                Button {
                    id: button_callerID
                    property bool pushed: false
                    height: 47
                    text: "Set IDs"
                    Material.background: "green"
                    Material.foreground: "white"
                    onClicked: {
                        pushed = !pushed
                        if (pushed) {
                            Material.background = "red"
                            text = "IDs are set"
                            rtc.setId(textfield_callerID.text)
                            rtc.setTargetId(textfield_callingID.text)
                            rtc.connectToSignalingServer()
                            rtc.init()
                            rtc.callOnRun()
                            rtc.createOffer()
                        } else {
                            Material.background = "green"
                            text = "Set Caller ID"
                            textfield_callerID.clear()
                            // Add disconnection logic here later
                        }
                    }
                }
            }

            // Row for Calling ID
            RowLayout {
                spacing: 10

                TextField {
                    id: textfield_callingID
                    placeholderText: "Calling ID"
                    Layout.fillWidth: true
                    enabled: !callbtn.pushed
                }

                Button {
                    id: callbtn
                    property bool pushed: false
                    height: 47
                    text: "Call"
                    Material.background: "green"
                    Material.foreground: "white"
                    onClicked: {
                        pushed = !pushed
                        if (pushed) {
                            Material.background = "red"
                            text = "End Call"
                            rtc.sendOffer()
                        } else {
                            Material.background = "green"
                            text = "Call"
                            textfield_callingID.clear()
                        }
                    }
                }
            }
        }

        Connections {
            target: rtc
            onDebugMessage: logOutput.text += message + "\n"
        }

//        Connections {
//            target: rtc
//            onAnswerIsReady: rtc.sendAnswer()
//        }
    }
}
