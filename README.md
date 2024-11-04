# Computer Network, Assignment 1
# Implementing a distributed voice call using WebRTC
Fatemeh (Shakiba) Baharvand
## Explanation
### WebRTC
![image](https://github.com/user-attachments/assets/5ca390de-2b6e-418c-896a-71b92502a722)

- **WebRTC** stands for Web Real-Time Communication, an open-source technology.
- It allows real-time video, audio, and data sharing directly between web browsers and mobile applications.
- WebRTC creates a peer-to-peer (P2P) connection, so data can be sent directly between users.
- There is no need for intermediary servers to relay data, which reduces delays and improves performance.
- Applications of WebRTC include video conferencing, online gaming, remote collaboration, and telemedicine.
- WebRTC’s main features are:
  - Low-latency P2P connectivity that allows real-time communication.
  - Cross-platform compatibility with major browsers and mobile platforms.
  - End-to-end encryption for secure data sharing.
  - Support for audio and video capture and streaming.
- Implementing WebRTC requires several steps, including:
  - Setting up signaling to establish the initial connection between users.
  - Managing network issues like firewalls or NAT (Network Address Translation) that can block direct connections.
  - Handling codec compatibility for audio and video on different devices.
- Despite its challenges, WebRTC’s ecosystem has tools and libraries like to make development easier.
- With WebRTC, developers can build dynamic, real-time applications across web and mobile platforms without complex server setups.
**Ref:** https://medium.com/@nakrani/what-is-webrtc-6e9cc2a51222
#### SDP (Session Description Protocol)
SDP (Session Description Protocol) is a format for describing multimedia communication sessions. In WebRTC, SDP is used for:
- Negotiating Capabilities: Determines the codecs, media types, and formats that each peer supports.
- Establishing Media Session: SDP messages (Offer/Answer) are exchanged to establish parameters for audio, video, or data channels.
- ICE Candidates: ICE candidates in SDP help peers find the best route for data transmission.
### CoTURN
CoTURN is an open-source server for TURN and STUN protocols. It helps clients behind firewalls connect for voice, video, and data sharing. CoTURN relays data between the client and server, making the connection appear to come from the server. This is essential for apps needing real-time communication, like WebRTC, especially when direct peer-to-peer connections aren’t possible.
- STUN finds a device’s public IP behind a firewall. This allows direct connections between devices.
- TURN relays data when direct connections are blocked. TURN is critical for WebRTC’s reliable performance.
In WebRTC, CoTURN uses the ICE protocol for checking connectivity and setting up real-time connections, even through restrictive firewalls.
- Many enterprise firewalls only allow specific apps and websites. They block most UDP connections.
- These firewalls need authorized TURN servers, like CoTURN, for WebRTC media streams.
- CoTURN can separate business and social traffic using first and third-party authorization.
Advantages of CoTURN:
- Firewall Traversal: It allows media and data sharing in restricted networks.
- Reliability: CoTURN ensures reliable connections when direct peer-to-peer isn’t possible.
- Security: It supports encryption, helping protect data.
- Enterprise-Friendly: CoTURN works well with enterprise firewall policies, allowing controlled WebRTC connections.

## App
![image](https://github.com/user-attachments/assets/72a73bce-7787-4277-b5af-4dbd1bd6f660)
The objective of this application is to establish a voice call between two peers using WebRTC. The setup involves a signaling server that facilitates communication between the peers. One peer acts as the offerer, sending audio, while the other peer, the answerer, receives and plays the audio.
### Call Flow Outline
1. Initialize the Server: Start by running server.js, which acts as the signaling server, facilitating message exchange between peers.
2. Open Application Instances: Open two instances of the application (the exe file), one for each peer (offerer and answerer).
3. Set IDs and Start Call:
  - Set caller ID and target ID for each peer.
  - Click the call button on the offerer's side to initiate the connection.
  - Upon setting IDs, the peers will register with the signaling server.
4. Exchange SDP (Session Description Protocol):
  - Once registered, the offerer will create an SDP offer, which is sent to the signaling server.
  - The signaling server routes the offer SDP to the answerer.
  - The answerer generates an SDP answer and returns it to the offerer through the signaling server.
5. Set Local and Remote SDP:
  - Each peer sets the remote SDP received from the other peer.
  - ICE (Interactive Connectivity Establishment) candidates are exchanged for connectivity.
6. Establish Connection: Once local and remote SDPs are set and ICE gathering is completed, the peer connection is established and sending tracks starts.
7. Audio Capture and Transmission: The application captures audio from the microphone and sends it as RTP packets to the connected peer. Incoming audio packets from the peer are processed and emitted as signals for playback or further handling.
8. State Monitoring: The application monitors the state of the peer connection, handling changes and potential errors.
## Code Overview
### WebRTC
#### 1. Helper Function: `peerConnectionStateToString`

```cpp
std::string peerConnectionStateToString(rtc::PeerConnection::State state) {
    switch (state) {
    case rtc::PeerConnection::State::New: return "New";
    case rtc::PeerConnection::State::Connecting: return "Connecting";
    case rtc::PeerConnection::State::Connected: return "Connected";
    case rtc::PeerConnection::State::Disconnected: return "Disconnected";
    case rtc::PeerConnection::State::Failed: return "Failed";
    case rtc::PeerConnection::State::Closed: return "Closed";
    default: return "Unknown State";
    }
}
```

- This function converts the `PeerConnection` state enumeration into a human-readable string. This is useful for logging and debugging, making it easier to track the connection state during runtime.

#### 2. WebRTC Class Constructor

```cpp
WebRTC::WebRTC(QObject *parent)
    : QObject{parent},
      webSocket(new QWebSocket()),
      localSDP("")
{
    Q_EMIT debugMessage("[WebRTC] Constructor called: Initialization completed.");
    peerIsOfferer = false;
}
```

- The constructor initializes the WebRTC object, creates a new WebSocket instance, and sets up initial states such as `localSDP` and `peerIsOfferer`. The `debugMessage` signal is emitted to indicate that initialization has been completed.

#### 3. Getter and Setter Methods

```cpp
bool WebRTC::getIsOfferer(){
    return peerIsOfferer;
}

void WebRTC::setId(QString id) {
    peerId = id;
    Q_EMIT debugMessage("[WebRTC] Peer ID set to: " + peerId);
}

void WebRTC::setTargetId(QString id) {
    targetId = id;
    Q_EMIT debugMessage("[WebRTC] Target ID set to: " + targetId);
}
```

- **`getIsOfferer()`**: Returns whether this peer is set as the offerer in the WebRTC session.
- **`setId(QString id)` and `setTargetId(QString id)`**: Set the IDs for this peer and the target peer, respectively, emitting debug messages to log these changes.

#### 4. Initialization Method: `init`

```cpp
void WebRTC::init() {
    rtc::InitLogger(rtc::LogLevel::Debug);
    Q_EMIT debugMessage("[WebRTC] Initialization started.");

    config.iceServers.emplace_back("stun:stun.l.google.com:19302");
    config.iceServers.emplace_back("turn:openrelayproject:openrelayproject@openrelay.metered.ca:80");

    peerConnection = std::make_shared<rtc::PeerConnection>(config);
    Q_EMIT debugMessage("[WebRTC] ICE servers configured and peer connection created.");

    auto audio = rtc::Description::Audio("Audio",
                                         this->peerIsOfferer?
                                             rtc::Description::Direction::SendOnly : rtc::Description::Direction::RecvOnly);

    audio.setBitrate(48000);
    audio.addOpusCodec(payloadType);
    audio.addSSRC(ssrc, "shakiba");

    audioTrack = peerConnection->addTrack(audio);

    audioTrack->onMessage([this](rtc::message_variant data) {
        Q_EMIT debugMessage("[WebRTC] Data received in onMessage.");

        if (std::holds_alternative<std::vector<std::byte>>(data)) {
            const auto& rawData = std::get<std::vector<std::byte>>(data);
            constexpr size_t rtpHeaderSize = sizeof(RTPHeader);
            if (rawData.size() > rtpHeaderSize) {
                QByteArray audioData(reinterpret_cast<const char*>(rawData.data()) + rtpHeaderSize,
                                     static_cast<int>(rawData.size() - rtpHeaderSize));

                Q_EMIT audioDataReceived(audioData);
                Q_EMIT debugMessage("[WebRTC] Received audio packet and stripped RTP header. Data size: "
                                    + QString::number(audioData.size()) + " bytes.");
            } else {
                Q_EMIT debugMessage("[WebRTC] Received packet too small for RTP header removal.");
            }
        }

        audioTrack->onOpen([this](){
            Q_EMIT debugMessage("[WebRTC] Track is open now ================================================");
        });

        Q_EMIT debugMessage("[WebRTC] Audio track added to peer connection.");
    });
}
```

- **Initialization**: Sets up logging and ICE servers. The STUN and TURN servers are essential for establishing peer-to-peer connections through NAT (Network Address Translation) devices.
- **Peer Connection**: A new `PeerConnection` is created using the specified configuration.
- **Audio Description**: An audio track is created with specific configurations (bitrate, codec, SSRC).
- **Message Handling**: A lambda function is registered to handle incoming messages on the audio track. It extracts audio data from the received messages and emits a signal with the audio data, while also managing RTP header stripping.

#### 5. Audio Capture Handling

```cpp
void WebRTC::onAudioCaptured(const QByteArray &audioData) {
    Q_EMIT debugMessage("[WebRTC] onAudioCaptured.");
    if (audioTrack) {
        std::vector<std::byte> byteData(audioData.size());
        std::memcpy(byteData.data(), audioData.data(), audioData.size());
        audioTrack->send(byteData);
        Q_EMIT debugMessage("[WebRTC] Audio data sent. Size: " + QString::number(audioData.size()) + " bytes.");
    }
}
```

- **`onAudioCaptured`**: This method is called when audio is captured from the microphone. It converts the audio data from a `QByteArray` to a `std::vector<std::byte>` and sends it over the audio track.

#### 6. RTP Header Creation

```cpp
RTPHeader WebRTC::createRTPHeader(uint16_t sequenceNumber) {
    RTPHeader header;
    header.marker = 0;
    header.payloadType = payloadType;
    header.sequenceNumber = qToBigEndian(sequenceNumber);
    header.timestamp = qToBigEndian(getCurrentTimeStamp());
    header.ssrc = qToBigEndian(ssrc);

    Q_EMIT debugMessage("[WebRTC] Created RTP header: Version: " + QString::number(2) + ", Payload Type: " + QString::number(header.payloadType) + ", Sequence Number: " + QString::number(header.sequenceNumber) + ", Timestamp: " + QString::number(header.timestamp));

    return header;
}
```

- **`createRTPHeader`**: Constructs an RTP header with relevant fields, including payload type, sequence number, timestamp, and SSRC. The timestamp is obtained from the current time, adjusted for a sample rate of 48kHz. This function emits debug information about the created header.

#### 7. Call Control

```cpp
void WebRTC::callOnRun() {
    Q_EMIT debugMessage("[WebRTC] callOnRun initiated.");

    peerConnection->onLocalDescription([this](rtc::Description description) {
        Q_EMIT debugMessage("[WebRTC] Local SDP offer being created.");
        Q_EMIT localDescriptionGenerated(QString::fromStdString(description));
        isLocalDescriptionGenerated = true;
        localSDP = QString::fromStdString(description);
        QStringList sdpLines = localSDP.split('\n');
        for (int i = 0; i < sdpLines.size(); ++i) {
            if (sdpLines[i].startsWith("v=")) {
                if(peerIsOfferer){
                    sdpLines[i] = "v=1";
                } else {
                    sdpLines[i] = "v=2";
                }
                break;
            }
        }
        localSDP = sdpLines.join('\n');
        Q_EMIT debugMessage("[WebRTC] Local SDP generated and emitted\n"+localSDP);
    });

    peerConnection->onLocalCandidate([this](rtc::Candidate candidate) {
        QString candidateStr = QString::fromStdString(candidate);
        localIceCandidates.append(candidateStr);
        Q_EMIT localCandidateGenerated(candidateStr);
        Q_EMIT debugMessage("[WebRTC] Local ICE candidate generated and emitted: " + candidateStr);
    });

    peerConnection->onStateChange([this](rtc::PeerConnection::State state) {
        QString stateStr = QString::fromStdString(peerConnectionStateToString(state));
        Q_EMIT debugMessage("[WebRTC] PeerConnection state changed: " + stateStr);
        if (state == rtc::PeerConnection::State::Failed) {
            Q_EMIT debugMessage("[WebRTC] Warning: Connection failed; a TURN server might be required.");
        }
        if

 (state == rtc::PeerConnection::State::Closed) {
            Q_EMIT debugMessage("[WebRTC] PeerConnection closed. Cleanup initiated.");
            // Implement any cleanup logic here
        }
    });
}
```

- **`callOnRun`**: This method sets up callbacks for local SDP generation, local candidate gathering, and state changes of the `PeerConnection`.
  - **Local Description**: When the local SDP is created, it emits it via the `localDescriptionGenerated` signal.
  - **Local Candidate**: When a local ICE candidate is generated, it is stored and emitted through the `localCandidateGenerated` signal.
  - **State Change Handling**: Tracks the state of the peer connection, logging changes and any potential issues (e.g., connection failure).

#### 8. WebSocket State Checking

```cpp
void WebRTC::checkWebSocketState() {
    if(webSocket) {
        if(webSocket->state() == QAbstractSocket::ConnectedState) {
            Q_EMIT debugMessage("[WebRTC] WebSocket is connected.");
        } else {
            Q_EMIT debugMessage("[WebRTC] WebSocket is not connected.");
        }
    }
}
```

- **`checkWebSocketState`**: Monitors the state of the WebSocket connection, emitting debug messages based on whether the WebSocket is connected.

#### 9. Client Registration

```cpp
void WebRTC::registerClient() {
    if(webSocket) {
        QJsonObject registrationMessage;
        registrationMessage["action"] = "register";
        registrationMessage["id"] = peerId;

        QJsonDocument doc(registrationMessage);
        webSocket->send(doc.toJson());
        Q_EMIT debugMessage("[WebRTC] Registration message sent: " + QString::fromUtf8(doc.toJson()));
    }
}
```

- **`registerClient`**: Sends a registration message to the signaling server, including the peer ID. It constructs a JSON object and emits debug information about the sent message.

#### 10. Signaling Server Connection

```cpp
void WebRTC::connectToSignalingServer() {
    connect(webSocket, &QWebSocket::connected, this, &WebRTC::onWebSocketConnected);
    connect(webSocket, &QWebSocket::disconnected, this, &WebRTC::onWebSocketDisconnected);
    connect(webSocket, &QWebSocket::textMessageReceived, this, &WebRTC::onSignalingMessageReceived);
    webSocket->open(QUrl("ws://your.signaling.server:port"));

    Q_EMIT debugMessage("[WebRTC] Connecting to signaling server...");
}
```

- **`connectToSignalingServer`**: Sets up connections to handle events when the WebSocket connects, disconnects, or receives messages. It initiates the connection to the signaling server using the specified URL.

#### 11. Offer and Answer Sending

```cpp
void WebRTC::sendOffer() {
    QJsonObject offerMessage;
    offerMessage["action"] = "offer";
    offerMessage["sdp"] = localSDP;

    QJsonDocument doc(offerMessage);
    webSocket->send(doc.toJson());
    Q_EMIT debugMessage("[WebRTC] Offer sent: " + QString::fromUtf8(doc.toJson()));
}

void WebRTC::sendAnswer() {
    QJsonObject answerMessage;
    answerMessage["action"] = "answer";
    answerMessage["sdp"] = localSDP;

    QJsonDocument doc(answerMessage);
    webSocket->send(doc.toJson());
    Q_EMIT debugMessage("[WebRTC] Answer sent: " + QString::fromUtf8(doc.toJson()));
}
```

- **`sendOffer` and `sendAnswer`**: These methods construct JSON messages containing the SDP offer or answer and send them to the signaling server. Debug messages are emitted to log the sent offers and answers.

#### 12. Signaling Message Handling

```cpp
void WebRTC::onSignalingMessageReceived(const QString &message) {
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject json = doc.object();
    QString action = json["action"].toString();

    if (action == "offer") {
        QString sdp = json["sdp"].toString();
        Q_EMIT debugMessage("[WebRTC] Offer received: " + sdp);
        // Handle SDP offer reception logic, create an answer
    } else if (action == "answer") {
        QString sdp = json["sdp"].toString();
        Q_EMIT debugMessage("[WebRTC] Answer received: " + sdp);
        // Handle SDP answer reception logic
    } else if (action == "candidate") {
        QString candidate = json["candidate"].toString();
        Q_EMIT debugMessage("[WebRTC] ICE candidate received: " + candidate);
        addRemoteCandidate(candidate);
    } else {
        Q_EMIT debugMessage("[WebRTC] Unknown action received: " + action);
    }
}
```

- **`onSignalingMessageReceived`**: This method is triggered when a message is received from the signaling server. It parses the JSON message and handles different actions: offers, answers, and ICE candidates. Based on the action type, appropriate debug messages are emitted.

#### 13. Offer Creation

```cpp
void WebRTC::createOffer() {
    peerConnection->createOffer([this](rtc::Description offer) {
        Q_EMIT debugMessage("[WebRTC] Creating offer...");
        localSDP = offer.toString();
        sendOffer();
    }, [](const std::string& error) {
        Q_EMIT debugMessage("[WebRTC] Error creating offer: " + QString::fromStdString(error));
    });
}
```

- **`createOffer`**: Initiates the process of creating an SDP offer. When the offer is successfully created, it is converted to a string and sent using the `sendOffer` method. If an error occurs, it emits a debug message with the error details.

14. **Adding Remote Candidates**:

```cpp
void WebRTC::addRemoteCandidate(const QString& candidate) {
    // Assuming candidate string is in the appropriate format
    rtc::Candidate iceCandidate = rtc::Candidate::FromString(candidate.toStdString());
    peerConnection->addRemoteCandidate(iceCandidate);
    Q_EMIT debugMessage("[WebRTC] Remote ICE candidate added: " + candidate);
}
```

- **`addRemoteCandidate`**: Converts the received candidate string into an `rtc::Candidate` object and adds it to the peer connection. It emits a debug message confirming the addition.

15. **Track Sending**:

```cpp
void WebRTC::sendTrack(const QByteArray &audioData) {
    auto rtpHeader = createRTPHeader(sequenceNumber);
    sequenceNumber++;
    
    std::vector<std::byte> packet;
    packet.resize(sizeof(RTPHeader) + audioData.size());
    std::memcpy(packet.data(), &rtpHeader, sizeof(RTPHeader));
    std::memcpy(packet.data() + sizeof(RTPHeader), audioData.data(), audioData.size());

    audioTrack->send(packet);
    Q_EMIT debugMessage("[WebRTC] Track sent with sequence number: " + QString::number(sequenceNumber));
}
```

- **`sendTrack`**: This function constructs an RTP packet by creating a header and appending the audio data. It sends the complete packet over the audio track and logs the action with the current sequence number.

16. **Timestamp Calculation**:

```cpp
uint32_t WebRTC::getCurrentTimeStamp() {
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    // Assuming 48kHz sampling rate for audio
    return static_cast<uint32_t>(currentTime * 48);
}
```

- **`getCurrentTimeStamp`**: Retrieves the current timestamp in milliseconds since epoch and converts it to a format suitable for RTP, scaling it to the audio sampling rate (48 kHz).

This code establishes a WebRTC-based audio communication application capable of real-time peer-to-peer audio streaming. It includes mechanisms for signaling, connection management, audio capture, and transmission, making it a robust

**WebRTC class**
```
#ifndef WEBRTC_H
#define WEBRTC_H

#include <QObject>
#include <QWebSocket>
#include <memory>
#include <string>
#include "audioinput.h"
#include "rtc/rtc.hpp"
#include <QJsonDocument>
#include <cstdint>

#pragma pack(push, 1)
struct RTPHeader {
    uint8_t first = 0b10000000;
    uint8_t marker : 1;
    uint8_t payloadType : 7;
    uint16_t sequenceNumber;
    uint32_t timestamp;
    uint32_t ssrc;
};
#pragma pack(pop)

class WebRTC : public QObject {
    Q_OBJECT

public:
    explicit WebRTC(QObject *parent = nullptr);
    Q_INVOKABLE void setId(QString id);
    Q_INVOKABLE void init();
    Q_INVOKABLE void registerClient();
    Q_INVOKABLE void connectToSignalingServer();
    Q_INVOKABLE void createOffer();
    Q_INVOKABLE void callOnRun();
    Q_INVOKABLE void addRemoteCandidate(const QString& candidate);
    Q_INVOKABLE void setTargetId(QString id);
    Q_INVOKABLE void checkWebSocketState();
    Q_INVOKABLE bool getIsOfferer();
    Q_INVOKABLE RTPHeader createRTPHeader(uint16_t sequenceNumber);
    Q_INVOKABLE uint32_t getCurrentTimeStamp();

public:
    bool getPeerIsOfferer() const;
    void setPeerIsOfferer(bool newPeerIsOfferer);
    void resetPeerIsOfferer();

public:Q_SIGNALS:
    void localDescriptionGenerated(const QString &sdp);
    void localCandidateGenerated(const QString &candidate);
    void audioReceived(const QByteArray &data);
    void gatheringCompleted();
    void debugMessage(QString message);
    void clientIsRegistered();
    void answerIsReady();
    void peerIsConnected();
    void audioDataReceived(const QByteArray &data);
    void peerIsOffererChanged();

public Q_SLOTS:
    void onSignalingServerConnected();
    void onSignalingServerDisconnected();
    void onSignalingMessageReceived(const QString &message);
    void sendOffer();
    void sendAnswer();
    void onAudioCaptured(const QByteArray &audioData);
    void sendTrack(const QByteArray &audioData);

private:
    std::shared_ptr<rtc::PeerConnection> peerConnection;
    std::shared_ptr<rtc::Track> audioTrack;
    std::shared_ptr<QWebSocket> webSocket;
    rtc::Description::Audio audio;
    rtc::Configuration config;
    QString peerId;
    QString targetId;
    bool peerIsOfferer;
    QString localSDP;
    QString remoteSDP;
    bool isClientRegistered = false;
    bool isLocalDescriptionGenerated = false;
    QStringList localIceCandidates;
    QStringList remoteIceCandidates;
    uint16_t sequenceNumber = 0;
    uint32_t ssrc = 2;
    uint32_t payloadType = 111;

    Q_PROPERTY(bool peerIsOfferer READ getPeerIsOfferer WRITE setPeerIsOfferer RESET resetPeerIsOfferer NOTIFY peerIsOffererChanged)
};

#endif // WEBRTC_H

```
### QML

It integrates WebRTC for real-time audio communication along with `AudioInput` and `AudioOutput` for capturing and playing audio. 
```
import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import WebRTC
import AudioInput
import AudioOutput

Window {
    width: 1010
    height: 520
    visible: true
    title: qsTr("Voice Call App")

    WebRTC {
        id: rtc
        onPeerIsConnected: {
                if(rtc.peerIsOfferer){
                    audioInput.startCapture()
                }
            }
        onAudioDataReceived: {
            if(!rtc.peerIsOfferer){
                audioOutput.playAudio(data)
            }
        }
    }

    AudioInput {
        id: audioInput

        onAudioCaptured: {
                console.log("[QML] Audio data captured:", data.length);
                rtc.sendTrack(data)
            }

        Component.onCompleted: {
            console.log("AudioInput initialized.")
        }

    }

    AudioOutput {
        id: audioOutput
        Component.onCompleted: {
            console.log("AudioOutput Ready to play audio.")
        }
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

                            // audioInput.startCapture()
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
                            audioInput.stopCapture()
                        }
                    }
                }
            }
        }

        Connections {
            target: rtc
            onDebugMessage: logOutput.text += message + "\n"
        }

    }
}
```
1. **Window Setup**:
   - The main window is set with a title and dimensions. 

2. **WebRTC Component**:
   - Manages the connection and audio data handling.
   - Starts audio capture when the peer is connected and is the offerer.
   - Plays audio data when received, if not the offerer.

3. **AudioInput Component**:
   - Captures audio and emits signals when audio is captured.
   - Logs the length of the captured audio data.

4. **AudioOutput Component**:
   - Prepares to play audio when ready.

5. **Logging Area**:
   - A `ScrollView` with a `TextArea` is used to log debug messages.

6. **UI Controls**:
   - Two `RowLayout`s contain text fields for Caller ID and Calling ID, and buttons for setting IDs and making calls.
   - The button toggles states for setting IDs and making calls.

7. **Connections**:
   - A `Connections` element listens to signals from the WebRTC object to log debug messages.

### AudioInput
AudioInput implementation is for handling audio capture and encoding with Opus.
```
#include <opus.h>
#include "AudioInput.h"
#include <QDebug>

const int OPUS_SAMPLE_RATE = 48000;
const int OPUS_CHANNELS = 1;

AudioInput::AudioInput(QObject *parent) : QIODevice(parent) {
    Q_EMIT debugMessage("[AudioInput] Initializing...");
    qDebug() << "[AudioInput] Initializing...";

    QAudioFormat format;
    format.setSampleRate(OPUS_SAMPLE_RATE);
    format.setChannelCount(OPUS_CHANNELS);
    format.setSampleFormat(QAudioFormat::Int16);

    audioSource = new QAudioSource(format, this);

    int error;
    opusEncoder = opus_encoder_create(OPUS_SAMPLE_RATE, OPUS_CHANNELS, OPUS_APPLICATION_AUDIO, &error);
    if (error != OPUS_OK) {
        Q_EMIT debugMessage("[AudioInput] Failed to initialize Opus encoder");
        qDebug() << "[AudioInput] Failed to initialize Opus encoder";
    } else {
        opus_encoder_ctl(opusEncoder, OPUS_SET_BITRATE(128000));
        opus_encoder_ctl(opusEncoder, OPUS_SET_COMPLEXITY(10));
        opus_encoder_ctl(opusEncoder, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
        Q_EMIT debugMessage("[AudioInput] Opus encoder initialized successfully");
        qDebug() << "[AudioInput] Opus encoder initialized successfully";
    }
}

AudioInput::~AudioInput() {
    opus_encoder_destroy(opusEncoder);
    audioSource->stop();
}

void AudioInput::stopCapture(){
    audioSource->stop();
}
void AudioInput::startCapture() {
    Q_EMIT debugMessage("[AudioInput] Starting capture...");
    qDebug() << "[AudioInput] Starting capture...";

    audioDevice = audioSource->start(); // Start capturing audio
    if (!audioDevice) {
        Q_EMIT debugMessage("[AudioInput] Failed to start audio source.");
        qDebug() << "[AudioInput] Failed to start audio source.";
        return;
    }

    // Connect readyRead to the slot or lambda function to handle incoming audio data
    connect(audioDevice, &QIODevice::readyRead, this, [this]() {
        QByteArray audioData = audioDevice->readAll();
        Q_EMIT debugMessage("[AudioInput] Captured raw audio data size: " + QString::number(audioData.size()));
        qDebug() << "[AudioInput] Captured raw audio data size:" << audioData.size();

        QByteArray encodedData = encodeAudio(audioData);
        Q_EMIT debugMessage("[AudioInput] Captured processed audio data size: " + QString::number(encodedData.size()));
        qDebug() << "[AudioInput] Captured processed audio data size:" << encodedData.size();

        if (encodedData.size() > 0) {
            Q_EMIT audioCaptured(encodedData);
        }
    });
}

qint64 AudioInput::readData(char *data, qint64 maxlen) {
    Q_UNUSED(data);
    Q_UNUSED(maxlen);
    return 0;
}

qint64 AudioInput::writeData(const char *data, qint64 len) {
    Q_EMIT audioCaptured(QByteArray(data, len));
    return len;
}

QByteArray AudioInput::encodeAudio(const QByteArray &input) {
    QByteArray output;
//    int maxEncodedBytes = 4000;
    //    unsigned char encodedData[maxEncodedBytes];
    std::vector<unsigned char> encodedData(input.size());    // Frame size calculation based on input data size
    //    int frameSize = input.size() / (OPUS_CHANNELS * sizeof(int16_t));
    int frameSize = input.size() / 2;
    int encodedBytes = opus_encode(opusEncoder, reinterpret_cast<const opus_int16*>(input.constData()), frameSize, encodedData.data(), encodedData.size());
    if (encodedBytes > 0) {
        output.append(reinterpret_cast<const char*>(encodedData.data()), encodedBytes);
    }
    return output;
}

```
1. **Initialization**:
The constructor initializes the audio format for the Opus encoder.
A QAudioSource is created with the specified format, and an Opus encoder is instantiated.
Various Opus encoder controls are set (bitrate, complexity, signal type).

3. **Destructor**:
The destructor cleans up the Opus encoder and stops the audio source.

5. **Starting and Stopping Capture**:
startCapture() method begins capturing audio from the source and connects the readyRead signal of the audio device to a lambda function that processes the audio data.
stopCapture() stops the audio source.

7. **Data Handling**:
The readData() and writeData() methods are overridden but only writeData() emits the captured audio data.
encodeAudio() handles the encoding of raw audio data into Opus format.

9. **Encoding Logic**:
It uses the opus_encode function to encode the captured raw audio data. The function returns the size of the encoded data, which is then emitted if it's greater than zero.

**AudioInput Class**
```
#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QIODevice>
#include <QAudioFormat>
#include <QAudioSource>
#include <opus.h>

class AudioInput : public QIODevice {
    Q_OBJECT

public:
    explicit AudioInput(QObject *parent = nullptr);
    ~AudioInput() override;
    Q_INVOKABLE void startCapture();
    Q_INVOKABLE void stopCapture();

public: Q_SIGNALS:
    void audioCaptured(const QByteArray &data);
    void debugMessage(const QString &message);

protected:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

private:
    QByteArray encodeAudio(const QByteArray &input);

    QAudioSource *audioSource;
    QIODevice *audioDevice;
    OpusEncoder *opusEncoder;
};

#endif // AUDIOINPUT_H
```
### AudioOutput
The AudioOutput class manages audio playback by decoding Opus-encoded audio data and outputting it through a specified audio sink, ensuring thread-safe operation and resource management.
```
#include <opus.h>
#include "AudioOutput.h"
#include <QDebug>

const int OPUS_SAMPLE_RATE = 48000;
const int OPUS_CHANNELS = 1;
const int OPUS_FRAME_SIZE = 960;

AudioOutput::AudioOutput(QObject *parent) : QObject(parent) {
    QAudioFormat format;
    format.setSampleRate(OPUS_SAMPLE_RATE);
    format.setChannelCount(OPUS_CHANNELS);
    format.setSampleFormat(QAudioFormat::Int16);

    Q_EMIT debugMessage("AudioOutput initializing...");
    qDebug() << "AudioOutput initializing...";

    audioSink = new QAudioSink(format, this);
    outputDevice = audioSink->start();

    if (!outputDevice) {
        Q_EMIT debugMessage("[AudioOutput] Failed to detect audio output device.");
        qDebug() << "[AudioOutput] Failed to detect audio output device.";
    } else {
        Q_EMIT debugMessage("[AudioOutput] Audio output device detected.");
        qDebug() << "[AudioOutput] Audio output device detected.";
    }

    int error;
    opusDecoder = opus_decoder_create(OPUS_SAMPLE_RATE, OPUS_CHANNELS, &error);
    if (error != OPUS_OK) {
        Q_EMIT debugMessage("[AudioOutput] Failed to initialize Opus decoder.");
        qDebug() << "[AudioOutput] Failed to initialize Opus decoder.";
    } else {
        Q_EMIT debugMessage("[AudioOutput] Opus decoder initialized successfully.");
        qDebug() << "[AudioOutput] Opus decoder initialized successfully.";
    }
}

AudioOutput::~AudioOutput() {
    opus_decoder_destroy(opusDecoder);
    audioSink->stop();
}

void AudioOutput::playAudio(const QByteArray &data) {
    QMutexLocker locker(&mutex);

    QByteArray decodedData = decodeAudio(data);
    if (!decodedData.isEmpty()) {
        qint64 writtenBytes = outputDevice->write(decodedData);
        Q_EMIT debugMessage("[AudioOutput] Playing audio, written bytes: " + QString::number(writtenBytes));
        qDebug() << "[AudioOutput] Playing audio, written bytes:" << writtenBytes;
    } else {
        Q_EMIT debugMessage("[AudioOutput] No data decoded to play.");
        qDebug() << "[AudioOutput] No data decoded to play.";
    }
}

QByteArray AudioOutput::decodeAudio(const QByteArray &input) {
    QByteArray output;
    int maxDecodedSamples = OPUS_FRAME_SIZE;
//    opus_int16 decodedData[maxDecodedSamples];

    std::vector<opus_int16> decodedData(maxDecodedSamples);
    int decodedSamples = opus_decode(opusDecoder, reinterpret_cast<const unsigned char*>(input.data()), input.size(), decodedData.data(), maxDecodedSamples, 0);
    if (decodedSamples > 0) {
        output.append(reinterpret_cast<const char*>(decodedData.data()), decodedSamples * sizeof(int16_t));
        Q_EMIT debugMessage("[AudioOutput] Decoded audio data size: " + QString::number(decodedSamples * sizeof(int16_t)));
        qDebug() << "[AudioOutput] Decoded audio data size:" << decodedSamples * sizeof(int16_t);
    } else {
        Q_EMIT debugMessage("[AudioOutput] Decoding failed, samples decoded: " + QString::number(decodedSamples));
        qDebug() << "[AudioOutput] Decoding failed, samples decoded:" << decodedSamples;
    }
    return output;
}

```
1. **Initialization**:
  - This section outlines the initialization of the `AudioOutput` class, which includes setting up the audio format using `QAudioFormat`, creating an `audioSink` for output, and starting the audio output device. It also checks for successful device detection and initializes the Opus decoder while logging any errors encountered.
2. **Destructor**:
  - The destructor is responsible for cleaning up resources. It destroys the Opus decoder to prevent memory leaks and stops the audio sink to cease audio output.
3. **Play Audio**:
  - The `playAudio` method is designed to handle the playback of audio. It locks a mutex to ensure thread safety, decodes the audio data received as a `QByteArray`, and writes it to the audio output device. The method logs the number of bytes written, and if there is no data to play, it logs that no decoded data is available.
4. **Decode Audio**:
  - This method performs the actual decoding of audio data. It uses the Opus library to decode the audio from its compressed format. If successful, the decoded samples are appended to an output `QByteArray`. The method also logs the size of the decoded data or reports a failure if decoding does not succeed.

**AudioOutput Class**
```
#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QObject>
#include <QAudioFormat>
#include <QAudioSink>
#include <QMutex>
#include <opus.h>

class AudioOutput : public QObject {
    Q_OBJECT

public:
    explicit AudioOutput(QObject *parent = nullptr);
    ~AudioOutput() override;

    Q_INVOKABLE void playAudio(const QByteArray &data);

public: Q_SIGNALS:
    void debugMessage(const QString &message);

private:
    QByteArray decodeAudio(const QByteArray &input);

    QAudioSink *audioSink;
    QIODevice *outputDevice;
    QMutex mutex;
    OpusDecoder *opusDecoder;
};

#endif // AUDIOOUTPUT_H

```

### Main
This code is essential for initializing the application and making the audio capture and playback functionalities accessible within the QML environment, facilitating the development of a voice call application.
```
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCoreApplication>
#include <QDebug>
#include <QObject>
#include "webrtc.h"
#include "audioinput.h"
#include "audiooutput.h"
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    const QUrl url(u"qrc:/Main.qml"_qs);
    qmlRegisterType<WebRTC>("WebRTC", 1, 0, "WebRTC");
    qmlRegisterType<AudioInput>("AudioInput", 1, 0, "AudioInput");
    qmlRegisterType<AudioOutput>("AudioOutput", 1, 0, "AudioOutput");
    qRegisterMetaType<QByteArray>("QByteArray");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() {QCoreApplication::exit(-1);}, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

//#include "main.moc"
```

## Signaling Server
This code sets up a WebSocket signaling server using Node.js for handling WebRTC connections.
```
const WebSocket = require('ws');

const wss = new WebSocket.Server({ port: 3000 });
let clients = {};

wss.on('connection', (ws) => {
    console.log("[Server] New client connected");

    ws.on('message', (message) => {
        const data = JSON.parse(message);
        console.log(`[Server] Received message of type: ${data.type} from client: ${data.id}`);

        switch (data.type) {
            case 'register':
                clients[data.id] = ws;
                console.log(`[Server] Client registered with ID: ${data.id}`);
                break;
            case 'offer':
                handleOffer(data);
                break;
            case 'answer':
                handleAnswer(data);
                break;
            case 'candidate':
                handleCandidate(data);
                break;
            case 'disconnect':
                handleDisconnect(data.id);
                break;
            default:
                console.log(`[Server] Unknown message type: ${data.type}`);
        }
    });

    ws.on('close', () => {
        removeClient(ws);
    });
});

function handleOffer(data) {
    console.log(`[Server] Forwarding offer from ${data.id} to ${data.targetId}`);
    const offerTarget = clients[data.targetId];
    if (offerTarget) {
        offerTarget.send(JSON.stringify(data));
        console.log(`[Server] Offer sent to ${data.targetId}`);
    } else {
        console.log(`[Server] Target client ${data.targetId} not found`);
    }
}

function handleAnswer(data) {
    console.log(`[Server] Forwarding answer from ${data.id} to ${data.targetId}`);
    const answerTarget = clients[data.targetId];
    if (answerTarget) {
        answerTarget.send(JSON.stringify(data));
        console.log(`[Server] Answer sent to ${data.targetId}`);
    } else {
        console.log(`[Server] Target client ${data.targetId} not found`);
    }
}

function handleCandidate(data) {
    console.log(`[Server] Forwarding candidate from ${data.id} to ${data.targetId}`);
    const candidateTarget = clients[data.targetId];
    if (candidateTarget) {
        candidateTarget.send(JSON.stringify(data));
        console.log(`[Server] Candidate sent to ${data.targetId}`);
    } else {
        console.log(`[Server] Target client ${data.targetId} not found`);
    }
}

function handleDisconnect(id) {
    delete clients[id];
    console.log(`[Server] Client ${id} disconnected`);
}

function removeClient(ws) {
    for (let id in clients) {
        if (clients[id] === ws) {
            console.log(`[Server] Client ${id} has disconnected`);
            delete clients[id];
            break;
        }
    }
}

console.log('Signaling server running on ws://localhost:3000');
```
1. **WebSocket Server Creation**:
   - The server is created to listen on port `3000` for incoming WebSocket connections.
2. **Client Management**:
   - A `clients` object is used to store connected clients by their IDs. This enables the server to route messages between them.
3. **Connection Handling**:
   - When a new client connects, a message is logged.
   - The server listens for incoming messages from clients. Each message is expected to be in JSON format.
4. **Message Handling**:
   - The server handles different message types:
     - **`register`**: Registers a client with a unique ID and stores the WebSocket connection.
     - **`offer`**: Forwards the WebRTC offer to the target client specified in the message.
     - **`answer`**: Forwards the WebRTC answer to the specified target client.
     - **`candidate`**: Forwards ICE candidates for connection establishment to the target client.
     - **`disconnect`**: Removes the client from the list of connected clients.
   - If an unknown message type is received, it logs a message indicating the issue.
5. **Client Disconnection**:
   - When a client disconnects, the server removes them from the `clients` object and logs the event.
6. **Logging**:
   - Various logs provide insights into the server’s activity, such as when clients connect, send messages, or disconnect.

## Output

![Uploading image.png…]()
### Caller
```
[WebRTC] Peer ID set to: 1
[WebRTC] Target ID set to: 2
[WebRTC] Attempting to connect to signaling server at ws://localhost:3000.
[WebRTC] Initialization started.
[WebRTC] ICE servers configured and peer connection created.
[WebRTC] callOnRun initiated.
[WebRTC] Creating new SDP offer.
[WebRTC] Local SDP offer being created.
[WebRTC] Local SDP generated and emitted
v=2
o=rtc 354418570 0 IN IP4 127.0.0.1
s=-
t=0 0
a=group:BUNDLE Audio
a=group:LS Audio
a=msid-semantic:WMS *
a=ice-options:ice2,trickle
a=fingerprint:sha-256 0A:08:80:FC:41:87:5C:1A:9F:C7:18:A0:D3:2A:AB:F3:31:C1:CC:2F:1F:58:CE:91:1B:C6:F7:92:62:D4:E0:5F
m=audio 9 UDP/TLS/RTP/SAVPF 111
c=IN IP4 0.0.0.0
b=AS:48000
a=mid:Audio
a=recvonly
a=ssrc:2 cname:shakiba
a=rtcp-mux
a=rtpmap:111 opus/48000/2
a=fmtp:111 minptime=10;maxaveragebitrate=96000;stereo=1;sprop-stereo=1;useinbandfec=1
a=setup:actpass
a=ice-ufrag:p80O
a=ice-pwd:6DuBZhzc8mK0Md1Ld0Y+DA

[WebRTC] Local ICE candidate generated and emitted: a=candidate:1 1 UDP 2122317823 192.168.167.59 65179 typ host
[WebRTC] PeerConnection state changed: Connecting
[WebRTC] Successfully connected to signaling server.
[WebRTC] Registration message sent for Peer ID: 1
[WebRTC] Current WebSocket state: ConnectedState
[WebRTC] Local ICE candidate generated and emitted: a=candidate:2 1 UDP 1686109951 204.18.183.224 65179 typ srflx raddr 0.0.0.0 rport 0
[WebRTC] The peer is set to offerer.
[WebRTC] SDP Offer sent to signaling server: v=1
o=rtc 354418570 0 IN IP4 127.0.0.1
s=-
t=0 0
a=group:BUNDLE Audio
a=group:LS Audio
a=msid-semantic:WMS *
a=ice-options:ice2,trickle
a=fingerprint:sha-256 0A:08:80:FC:41:87:5C:1A:9F:C7:18:A0:D3:2A:AB:F3:31:C1:CC:2F:1F:58:CE:91:1B:C6:F7:92:62:D4:E0:5F
m=audio 9 UDP/TLS/RTP/SAVPF 111
c=IN IP4 0.0.0.0
b=AS:48000
a=mid:Audio
a=recvonly
a=ssrc:2 cname:shakiba
a=rtcp-mux
a=rtpmap:111 opus/48000/2
a=fmtp:111 minptime=10;maxaveragebitrate=96000;stereo=1;sprop-stereo=1;useinbandfec=1
a=setup:actpass
a=ice-ufrag:p80O
a=ice-pwd:6DuBZhzc8mK0Md1Ld0Y+DA

[WebRTC] Signaling message received from server: {"id":"2","sdp":"v=2\no=rtc 3807741146 0 IN IP4 127.0.0.1\r\ns=-\r\nt=0 0\r\na=group:BUNDLE Audio\r\na=group:LS Audio\r\na=msid-semantic:WMS *\r\na=ice-options:ice2,trickle\r\na=fingerprint:sha-256 A1:97:9E:78:EB:85:F3:0A:A3:F4:DB:40:33:7E:81:BF:23:25:52:C6:7C:C3:0A:56:08:C7:FF:49:A4:F4:22:93\r\nm=audio 9 UDP/TLS/RTP/SAVPF 111\r\nc=IN IP4 0.0.0.0\r\nb=AS:48000\r\na=mid:Audio\r\na=recvonly\r\na=ssrc:2 cname:shakiba\r\na=rtcp-mux\r\na=rtpmap:111 opus/48000/2\r\na=fmtp:111 minptime=10;maxaveragebitrate=96000;stereo=1;sprop-stereo=1;useinbandfec=1\r\na=setup:active\r\na=ice-ufrag:gRak\r\na=ice-pwd:DZSn7h1/Z+xslstfI/zgjA\r\na=candidate:1 1 UDP 2122317823 192.168.167.59 65182 typ host\r\na=candidate:2 1 UDP 1686109951 204.18.183.224 65182 typ srflx raddr 0.0.0.0 rport 0\r\n","targetId":"1","type":"answer"}
[WebRTC] Remote SDP set for answer.
[WebRTC] PeerConnection state changed: Connected
[WebRTC] Success: Connection is set!
[WebRTC] Track is not open.
[WebRTC] Preparing to send track data. Size: 3 bytes.
[WebRTC] Created RTP header: Version: 2, Payload Type: 111, Sequence Number: 0, Timestamp: 1622764614
***************************************************************
[WebRTC] RTP packet sent successfully. Size: 15 bytes. Sequence Number: 0
[WebRTC] Updated timestamp for next packet: 1622764614
[WebRTC] Preparing to send track data. Size: 3 bytes.
[WebRTC] Created RTP header: Version: 2, Payload Type: 111, Sequence Number: 256, Timestamp: 1086221382
***************************************************************
[WebRTC] RTP packet sent successfully. Size: 15 bytes. Sequence Number: 256
[WebRTC] Updated timestamp for next packet: 1086221382
[WebRTC] Preparing to send track data. Size: 3 bytes.
[WebRTC] Created RTP header: Version: 2, Payload Type: 111, Sequence Number: 512, Timestamp: 1891920966
***************************************************************
[WebRTC] RTP packet sent successfully. Size: 15 bytes. Sequence Number: 512
[WebRTC] Updated timestamp for next packet: 1891920966
[WebRTC] Preparing to send track data. Size: 3 bytes.
[WebRTC] Created RTP header: Version: 2, Payload Type: 111, Sequence Number: 768, Timestamp: 1623747654
***************************************************************
[WebRTC] RTP packet sent successfully. Size: 15 bytes. Sequence Number: 768
[WebRTC] Updated timestamp for next packet: 1623747654
[WebRTC] Preparing to send track data. Size: 3 bytes.
[WebRTC] Created RTP header: Version: 2, Payload Type: 111, Sequence Number: 1024, Timestamp: 2966187078
***************************************************************
```
### Callee
```
[WebRTC] Peer ID set to: 2
[WebRTC] Target ID set to: 
[WebRTC] Attempting to connect to signaling server at ws://localhost:3000.
[WebRTC] Initialization started.
[WebRTC] ICE servers configured and peer connection created.
[WebRTC] callOnRun initiated.
[WebRTC] Creating new SDP offer.
[WebRTC] Local SDP offer being created.
[WebRTC] Local SDP generated and emitted
v=2
o=rtc 1830082462 0 IN IP4 127.0.0.1
s=-
t=0 0
a=group:BUNDLE Audio
a=group:LS Audio
a=msid-semantic:WMS *
a=ice-options:ice2,trickle
a=fingerprint:sha-256 A1:97:9E:78:EB:85:F3:0A:A3:F4:DB:40:33:7E:81:BF:23:25:52:C6:7C:C3:0A:56:08:C7:FF:49:A4:F4:22:93
m=audio 9 UDP/TLS/RTP/SAVPF 111
c=IN IP4 0.0.0.0
b=AS:48000
a=mid:Audio
a=recvonly
a=ssrc:2 cname:shakiba
a=rtcp-mux
a=rtpmap:111 opus/48000/2
a=fmtp:111 minptime=10;maxaveragebitrate=96000;stereo=1;sprop-stereo=1;useinbandfec=1
a=setup:actpass
a=ice-ufrag:gRak
a=ice-pwd:DZSn7h1/Z+xslstfI/zgjA

[WebRTC] Local ICE candidate generated and emitted: a=candidate:1 1 UDP 2122317823 192.168.167.59 65182 typ host
[WebRTC] PeerConnection state changed: Connecting
[WebRTC] Successfully connected to signaling server.
[WebRTC] Registration message sent for Peer ID: 2
[WebRTC] Current WebSocket state: ConnectedState
[WebRTC] Local ICE candidate generated and emitted: a=candidate:2 1 UDP 1686109951 204.18.183.224 65182 typ srflx raddr 0.0.0.0 rport 0
[WebRTC] Signaling message received from server: {"id":"1","sdp":"v=1\no=rtc 354418570 0 IN IP4 127.0.0.1\r\ns=-\r\nt=0 0\r\na=group:BUNDLE Audio\r\na=group:LS Audio\r\na=msid-semantic:WMS *\r\na=ice-options:ice2,trickle\r\na=fingerprint:sha-256 0A:08:80:FC:41:87:5C:1A:9F:C7:18:A0:D3:2A:AB:F3:31:C1:CC:2F:1F:58:CE:91:1B:C6:F7:92:62:D4:E0:5F\r\nm=audio 9 UDP/TLS/RTP/SAVPF 111\r\nc=IN IP4 0.0.0.0\r\nb=AS:48000\r\na=mid:Audio\r\na=recvonly\r\na=ssrc:2 cname:shakiba\r\na=rtcp-mux\r\na=rtpmap:111 opus/48000/2\r\na=fmtp:111 minptime=10;maxaveragebitrate=96000;stereo=1;sprop-stereo=1;useinbandfec=1\r\na=setup:actpass\r\na=ice-ufrag:p80O\r\na=ice-pwd:6DuBZhzc8mK0Md1Ld0Y+DA\r\n","targetId":"2","type":"offer"}
[WebRTC] Received offer with SDP: v=1
o=rtc 354418570 0 IN IP4 127.0.0.1
s=-
t=0 0
a=group:BUNDLE Audio
a=group:LS Audio
a=msid-semantic:WMS *
a=ice-options:ice2,trickle
a=fingerprint:sha-256 0A:08:80:FC:41:87:5C:1A:9F:C7:18:A0:D3:2A:AB:F3:31:C1:CC:2F:1F:58:CE:91:1B:C6:F7:92:62:D4:E0:5F
m=audio 9 UDP/TLS/RTP/SAVPF 111
c=IN IP4 0.0.0.0
b=AS:48000
a=mid:Audio
a=recvonly
a=ssrc:2 cname:shakiba
a=rtcp-mux
a=rtpmap:111 opus/48000/2
a=fmtp:111 minptime=10;maxaveragebitrate=96000;stereo=1;sprop-stereo=1;useinbandfec=1
a=setup:actpass
a=ice-ufrag:p80O
a=ice-pwd:6DuBZhzc8mK0Md1Ld0Y+DA

[WebRTC] Remote SDP set for offer.
[WebRTC] Creating new SDP answer.
[WebRTC] SDP Answer sent to signaling server: v=2
o=rtc 3807741146 0 IN IP4 127.0.0.1
s=-
t=0 0
a=group:BUNDLE Audio
a=group:LS Audio
a=msid-semantic:WMS *
a=ice-options:ice2,trickle
a=fingerprint:sha-256 A1:97:9E:78:EB:85:F3:0A:A3:F4:DB:40:33:7E:81:BF:23:25:52:C6:7C:C3:0A:56:08:C7:FF:49:A4:F4:22:93
m=audio 9 UDP/TLS/RTP/SAVPF 111
c=IN IP4 0.0.0.0
b=AS:48000
a=mid:Audio
a=recvonly
a=ssrc:2 cname:shakiba
a=rtcp-mux
a=rtpmap:111 opus/48000/2
a=fmtp:111 minptime=10;maxaveragebitrate=96000;stereo=1;sprop-stereo=1;useinbandfec=1
a=setup:active
a=ice-ufrag:gRak
a=ice-pwd:DZSn7h1/Z+xslstfI/zgjA
a=candidate:1 1 UDP 2122317823 192.168.167.59 65182 typ host
a=candidate:2 1 UDP 1686109951 204.18.183.224 65182 typ srflx raddr 0.0.0.0 rport 0

[WebRTC] Local SDP offer being created.
[WebRTC] Local SDP generated and emitted
v=2
o=rtc 3807741146 0 IN IP4 127.0.0.1
s=-
t=0 0
a=group:BUNDLE Audio
a=group:LS Audio
a=msid-semantic:WMS *
a=ice-options:ice2,trickle
a=fingerprint:sha-256 A1:97:9E:78:EB:85:F3:0A:A3:F4:DB:40:33:7E:81:BF:23:25:52:C6:7C:C3:0A:56:08:C7:FF:49:A4:F4:22:93
m=audio 9 UDP/TLS/RTP/SAVPF 111
c=IN IP4 0.0.0.0
b=AS:48000
a=mid:Audio
a=recvonly
a=ssrc:2 cname:shakiba
a=rtcp-mux
a=rtpmap:111 opus/48000/2
a=fmtp:111 minptime=10;maxaveragebitrate=96000;stereo=1;sprop-stereo=1;useinbandfec=1
a=setup:active
a=ice-ufrag:gRak
a=ice-pwd:DZSn7h1/Z+xslstfI/zgjA
a=candidate:1 1 UDP 2122317823 192.168.167.59 65182 typ host
a=candidate:2 1 UDP 1686109951 204.18.183.224 65182 typ srflx raddr 0.0.0.0 rport 0

[WebRTC] PeerConnection state changed: Connected
[WebRTC] Success: Connection is set!
[WebRTC] Track is not open.
[WebRTC] Data received in onMessage.
[WebRTC] Received audio packet and stripped RTP header. Data size: 3 bytes.
[WebRTC] Track is open now ================================================
[WebRTC] Audio track added to peer connection.
[WebRTC] Data received in onMessage.
[WebRTC] Received audio packet and stripped RTP header. Data size: 3 bytes.
[WebRTC] Audio track added to peer connection.
[WebRTC] Data received in onMessage.
[WebRTC] Received audio packet and stripped RTP header. Data size: 3 bytes.
[WebRTC] Audio track added to peer connection.
[WebRTC] Data received in onMessage.
[WebRTC] Received audio packet and stripped RTP header. Data size: 3 bytes.
[WebRTC] Audio track added to peer connection.
[WebRTC] Data received in onMessage.
```
