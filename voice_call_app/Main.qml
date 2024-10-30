import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import WebRTCClient

Window {
    width: 280
    height: 520
    visible: true
    title: qsTr("CA1")

    WebRTCClient {
            id: rtc
    }

    Item{
        anchors.fill: parent

        ColumnLayout {
            anchors{
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: textfield.top
                margins: 20
            }

            Label{
                text: "Ip: " + "172.16.142.176"
                Layout.fillWidth: true
                Layout.preferredHeight: 40
            }
            Label{
                text: "IceCandidate: " + "172.16.142.176"
                Layout.fillWidth: true
                Layout.preferredHeight: 40
            }
            Label{
                text: "CallerId: " + textfield.text
                Layout.fillWidth: true
                Layout.preferredHeight: 40
            }

        }

        TextField{
            id: textfield
            placeholderText: "Phone Number"
            anchors.bottom: callbtn.top
            anchors.bottomMargin: 10
            anchors.left: callbtn.left
            anchors.right: callbtn.right
            enabled: !callbtn.pushed
        }

        Button{
            id: callbtn

            property bool pushed: false

            height: 47
            text: "Call"
            Material.background: "green"
            Material.foreground: "white"
            anchors{
                bottom: parent.bottom
                left: parent.left
                right: parent.right
                margins: 20
            }

//            onClicked: {
//                pushed = !pushed
//                if(pushed){
//                    Material.background = "red"
//                    text = "End Call"
//                    /// * ADDED ----------------------
//                    webrtcClient.registerClient(textfield.text); // Register client with ID
//                    webrtcClient.sendOffer("your_sdp_offer", textfield.text, "your_id"); // Placeholder for SDP offer
//                }
//                else{
//                    Material.background = "green"
//                    text = "Call"
//                    /// * ADDED ----------------------
//                    webrtcClient.sendAnswer("your_sdp_answer", textfield.text, "your_id"); // Placeholder for SDP answer
//                    textfield.clear()

//                }
//            }
            onClicked: {
                pushed = !pushed
                if (pushed) {
                    Material.background = "red"
                    text = "End Call"
                    rtc.registerClient(textfield.text); // Register client with ID

                    // Assuming you have the correct values for targetId and offer
                    var targetId = textfield.text; // Use the caller ID or another identifier
                    var offer = "your_sdp_offer";   // Replace with actual SDP offer
                    rtc.sendOffer(targetId, offer); // Correct usage
                } else {
                    Material.background = "green"
                    text = "Call"

                    // Assuming you have the correct values for targetId and answer
                    var targetId = textfield.text; // Use the caller ID or another identifier
                    var answer = "your_sdp_answer"; // Replace with actual SDP answer
                    rtc.sendAnswer(targetId, answer); // Correct usage

                    textfield.clear();
                }
            }

        }
    }
}
