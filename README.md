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
### Code Overview

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

#### 14. Adding Remote Candidates

```cpp
void WebRTC::addRemoteCandidate(const QString& candidate) {
    // Assuming candidate string is in the appropriate format
    rtc::Candidate iceCandidate = rtc::Candidate::FromString(candidate.toStdString());
    peerConnection->addRemoteCandidate(iceCandidate);
    Q_EMIT debugMessage("[WebRTC] Remote ICE candidate added: " + candidate);
}
```

- **`addRemoteCandidate`**: Converts the received candidate string into an `rtc::Candidate` object and adds it to the peer connection. It emits a debug message confirming the addition.

#### 15. Track Sending

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

#### 16. Timestamp Calculation

```cpp
uint32_t WebRTC::getCurrentTimeStamp() {
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    // Assuming 48kHz sampling rate for audio
    return static_cast<uint32_t>(currentTime * 48);
}
```

- **`getCurrentTimeStamp`**: Retrieves the current timestamp in milliseconds since epoch and converts it to a format suitable for RTP, scaling it to the audio sampling rate (48 kHz).

This code establishes a WebRTC-based audio communication application capable of real-time peer-to-peer audio streaming. It includes mechanisms for signaling, connection management, audio capture, and transmission, making it a robust

#### The WebRTC class implementation:
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
