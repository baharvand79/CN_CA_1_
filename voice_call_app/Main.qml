//import QtQuick
//import QtQuick.Controls.Material
//import QtQuick.Layouts
//import WebRTC
//import Audio

//Window {
//    width: 280
//    height: 520
//    visible: true
//    title: qsTr("CA1")

//    AudioInput {
//        id: aIn

//        onReadyRead: (packet) => {
//                     }
//    }

//    AudioOutput{
//        id: aOut
//    }

//    RTC {
//        id: rtc
//    }

//    Item{
//        anchors.fill: parent

//        ColumnLayout {
//            anchors{
//                top: parent.top
//                left: parent.left
//                right: parent.right
//                bottom: textfield.top
//                margins: 20
//            }

//            Label{
//                text: "Ip: " + "172.16.142.176"
//                Layout.fillWidth: true
//                Layout.preferredHeight: 40
//            }
//            Label{
//                text: "IceCandidate: " + "172.16.142.176"
//                Layout.fillWidth: true
//                Layout.preferredHeight: 40
//            }
//            Label{
//                text: "CallerId: " + textfield.text
//                Layout.fillWidth: true
//                Layout.preferredHeight: 40
//            }

//        }

//        TextField{
//            id: textfield
//            placeholderText: "Phone Number"
//            anchors.bottom: callbtn.top
//            anchors.bottomMargin: 10
//            anchors.left: callbtn.left
//            anchors.right: callbtn.right
//            enabled: !callbtn.pushed
//        }

//        Button{
//            id: callbtn

//            property bool pushed: false

//            height: 47
//            text: "Call"
//            Material.background: "green"
//            Material.foreground: "white"
//            anchors{
//                bottom: parent.bottom
//                left: parent.left
//                right: parent.right
//                margins: 20
//            }

//            onClicked: {
//                pushed = !pushed
//                if(pushed){
//                    Material.background = "red"
//                    text = "End Call"
//                }
//                else{
//                    Material.background = "green"
//                    text = "Call"
//                    textfield.clear()
//                }
//            }
//        }
//    }
//}


import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import WebRTC
import Audio

Window {
    width: 280
    height: 520
    visible: true
    title: qsTr("CA1")

//    AudioInput {
//        id: aIn

//        onAudioCaptured: {
//            rtc.sendAudioData(encodedData); // Sending audio to WebRTC C++
//            console.log("[QML] Captured audio data sent to WebRTC.");
//        }
//    }

    AudioInput {
        id: aIn
        onReadyRead: {
            // Capture audio data, encode it, and send it
            var packet = audioDevice.readAll();
            var encodedData = encodeAudio(packet); // Ensure encodeAudio is a valid function
            if (encodedData) {
                rtc.sendAudio(encodedData); // Send to WebRTC
            }
        }
    }

    AudioOutput {
        id: aOut
    }

    RTC {
        id: rtc

        onAudioReceived: function(audioData) {
            aOut.playAudio(audioData); // Play received audio
            console.log("[QML] Received audio data played.");
        }
    }

    Item {
        anchors.fill: parent

        ColumnLayout {
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: textfield.top
                margins: 20
            }

            Label {
                text: "IP: 172.16.142.176"
                Layout.fillWidth: true
                Layout.preferredHeight: 40
            }
            Label {
                text: "IceCandidate: 172.16.142.176"
                Layout.fillWidth: true
                Layout.preferredHeight: 40
            }
            Label {
                text: "CallerId: " + textfield.text
                Layout.fillWidth: true
                Layout.preferredHeight: 40
            }
        }

        TextField {
            id: textfield
            placeholderText: "Phone Number"
            anchors.bottom: callbtn.top
            anchors.bottomMargin: 10
            anchors.left: callbtn.left
            anchors.right: callbtn.right
            enabled: !callbtn.pushed
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
                left: parent.left
                right: parent.right
                margins: 20
            }

            onClicked: {
                pushed = !pushed
                if (pushed) {
                    Material.background = "red"
                    text = "End Call"
                    aIn.startCapture();  // Start capturing audio
                    rtc.createOffer();    // Initiate WebRTC connection
                } else {
                    Material.background = "green"
                    text = "Call"
                    aIn.stopCapture();    // Stop capturing audio
                    // rtc.endCall();        // Disconnect WebRTC
                    textfield.clear();
                }
            }
        }
    }
}
